/* Copyright (c) 2012, Michael Patraw
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * The name of Michael Patraw may not be used to endorse or promote
 *       products derived from this software without specific prior written
 *       permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Michael Patraw ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Michael Patraw BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "at.h"
#include "at_aux.h"
#include "darray.h"
#include "can_see.h"
#include "mgenerator.h"
#include "random.h"

#define MWIDTH  160
#define MHEIGHT 80

#define IN_BOUNDS(x, y, w, h) ((x) >= 0 && (x) < (w) && (y) >= 0 && y < (h))

enum {
    TPLAYER,
    TFLOOR,
    TWALL,
    TCHEST,
    TTABLE,
    TMAX
};

struct tile {
    int symbol;
    int color;
    int obstructed;
};

struct world {
    int seen_map[MHEIGHT][MWIDTH];
    int obs_map[MHEIGHT][MWIDTH];
    int map[MHEIGHT][MWIDTH];

    int px, py;
};

struct tile tiles[TMAX] = {
    {'@', 0xff0000, 0},
    {'.', 0x777777, 0},
    {'#', 0x773300, 1},
    {'$', 0x776600, 0},
    {227, 0x774400, 0},
};

static void output_tile(int x, int y, int tile, int flags);
static void player_move(struct world *w, int dx, int dy);

static void clear_world(struct world *w, int tile);
static void generate_obstructed_map(struct world *w);
static void find_open_location(struct world *w, int *x, int *y);
static void border_map(struct world *w, int tile);

static void input(struct world *w);
static void output(struct world *w);

int room_XxX(mgenerator_type *mgen, int *map, int w, int h, int x, int y, int size);

int left_hall_5(mgenerator_type *mgen, int *map, int w, int h, int x, int y);
int right_hall_5(mgenerator_type *mgen, int *map, int w, int h, int x, int y);
int up_hall_5(mgenerator_type *mgen, int *map, int w, int h, int x, int y);
int down_hall_5(mgenerator_type *mgen, int *map, int w, int h, int x, int y);
int room_7x7(mgenerator_type *mgen, int *map, int w, int h, int x, int y);
int room_5x5(mgenerator_type *mgen, int *map, int w, int h, int x, int y);
int room_3x3(mgenerator_type *mgen, int *map, int w, int h, int x, int y);
int diamond_7(mgenerator_type *mgen, int *map, int w, int h, int x, int y);
int treasure_3x3(mgenerator_type *mgen, int *map, int w, int h, int x, int y);

rng_state rng;

int main(int argc, char *argv[])
{
    mgenerator_type mgen;
    struct world world;
    (void)argc;
    (void)argv;

    memset(&world, 0, sizeof(world));
    clear_world(&world, TWALL);

    rng_seed(&rng, time(NULL), RNG_CMWC);
    mgenerator_open(&mgen);
    mgenerator_add_plan(&mgen, left_hall_5, 4);
    mgenerator_add_plan(&mgen, right_hall_5, 4);
    mgenerator_add_plan(&mgen, up_hall_5, 4);
    mgenerator_add_plan(&mgen, down_hall_5, 4);
    mgenerator_add_plan(&mgen, room_7x7, 32);
    mgenerator_add_plan(&mgen, room_5x5, 16);
    mgenerator_add_plan(&mgen, room_3x3, 4);
    mgenerator_add_plan(&mgen, diamond_7, 4);
    mgenerator_add_plan(&mgen, treasure_3x3, 4);

    mgenerator_add_node(&mgen, rng_under(&rng, MWIDTH), rng_under(&rng, MHEIGHT));
    mgenerator_generate(&mgen, (int *)world.map, MWIDTH, MHEIGHT, 0x7fffffff);

    border_map(&world, TWALL);
    find_open_location(&world, &world.px, &world.py);
    generate_obstructed_map(&world);

    at_open(MWIDTH * AT_FWIDTH, MHEIGHT * AT_FHEIGHT, "at", 1, 1);

    while (at_flush()) {
        if (at_peek('Q'))
            break;
        if (at_peek('g')) {
            clear_world(&world, TWALL);

            mgenerator_add_node(&mgen, rng_under(&rng, MWIDTH), rng_under(&rng, MHEIGHT));
            mgenerator_generate(&mgen, (int *)world.map, MWIDTH, MHEIGHT, 0x7fffffff);

            border_map(&world, TWALL);
            find_open_location(&world, &world.px, &world.py);
            generate_obstructed_map(&world);
        }
        if (at_peek('s')) {
            memset(world.seen_map, 1, sizeof(world.seen_map));
        }
        input(&world);
        at_clear(0);
        output(&world);
    }

    mgenerator_close(&mgen);
    at_close();
    return 0;
}

static void output_tile(int x, int y, int tile, int flags)
{
    x *= AT_AUX_FWIDTH;
    y *= AT_AUX_FHEIGHT;

    at_plot_glyph(x, y, tiles[tile].symbol,
                  flags ? 0x222222 : tiles[tile].color);
}

static void player_move(struct world *w, int dx, int dy)
{
    if (IN_BOUNDS(w->px + dx, w->py + dy, MWIDTH, MHEIGHT) &&
        !tiles[w->map[w->py + dy][w->px + dx]].obstructed) {
        w->px += dx;
        w->py += dy;
    }
}

static void clear_world(struct world *w, int tile)
{
    int x, y;

    for (x = 0; x < MWIDTH; ++x) {
        for (y = 0; y < MHEIGHT; ++y) {
            w->map[y][x] = tile;
            w->seen_map[y][x] = 0;
            w->obs_map[y][x] = 0;
        }
    }
}

static void generate_obstructed_map(struct world *w)
{
    int x, y;

    for (x = 0; x < MWIDTH; ++x)
        for (y = 0; y < MHEIGHT; ++y)
            w->obs_map[y][x] = tiles[w->map[y][x]].obstructed;
}

static void find_open_location(struct world *w, int *x, int *y)
{
    int ix, iy;

    for (ix = 0; ix < MWIDTH; ++ix) {
        for (iy = 0; iy < MHEIGHT; ++iy) {
            if (!tiles[w->map[iy][ix]].obstructed) {
                *x = ix;
                *y = iy;
                return;
            }
        }
    }
}

static void border_map(struct world *w, int tile)
{
    int x, y;

    for (x = 0; x < MWIDTH; ++x) {
        w->map[0][x] = tile;
        w->map[MHEIGHT - 1][x] = tile;
    }

    for (y = 0; y < MHEIGHT; ++y) {
        w->map[y][0] = tile;
        w->map[y][MWIDTH - 1] = tile;
    }
}

static void input(struct world *w)
{
    if (at_peek('h'))
        player_move(w, -1, 0);
    else if (at_peek('j'))
        player_move(w, 0, 1);
    else if (at_peek('k'))
        player_move(w, 0, -1);
    else if (at_peek('l'))
        player_move(w, 1, 0);
    else if (at_peek('y'))
        player_move(w, -1, -1);
    else if (at_peek('u'))
        player_move(w, 1, -1);
    else if (at_peek('b'))
        player_move(w, -1, 1);
    else if (at_peek('n'))
        player_move(w, 1, 1);
}

static void output(struct world *w)
{
    int x, y;

    for (y = 0; y < MHEIGHT; ++y) {
        for (x = 0; x < MWIDTH; ++x) {
            if (can_see(w->px, w->py, x, y, 10, (int *)w->obs_map, MWIDTH)) {
                w->seen_map[y][x] = 1;
                output_tile(x, y, w->map[y][x], 0);
            }
            else if (w->seen_map[y][x]) {
                output_tile(x, y, w->map[y][x], 1);
            }
        }
    }

    output_tile(w->px, w->py, TPLAYER, 0);
}

int left_hall_5(mgenerator_type *mgen, int *map, int w, int h, int x, int y)
{
    int ix;

    for (ix = x; ix < x + 5; ++ix) {
        if (!IN_BOUNDS(ix, y, w, h) || map[y * w + ix] != TWALL)
            return -1;

        if (ix > x && ix + 1 < x + 5) {
            if (!IN_BOUNDS(ix, y + 1, w, h) || map[(y + 1) * w + ix] != TWALL)
                return -1;
            if (!IN_BOUNDS(ix, y - 1, w, h) || map[(y - 1) * w + ix] != TWALL)
                return -1;
        }
    }

    for (ix = x; ix < x + 5; ++ix)
        map[y * w + ix] = TFLOOR;

    mgenerator_add_node(mgen, x - 1, y);
    mgenerator_add_node(mgen, x + 5, y);
    mgenerator_add_node(mgen, x + rng_under(&rng, 5), y - 1);
    mgenerator_add_node(mgen, x + rng_under(&rng, 5), y + 1);

    return 0;
}

int right_hall_5(mgenerator_type *mgen, int *map, int w, int h, int x, int y)
{
    int ix;

    for (ix = x; ix > x - 5; --ix) {
        if (!IN_BOUNDS(ix, y, w, h) || map[y * w + ix] != TWALL)
            return -1;

        if (ix < x && ix - 1 > x - 5) {
            if (!IN_BOUNDS(ix, y + 1, w, h) || map[(y + 1) * w + ix] != TWALL)
                return -1;
            if (!IN_BOUNDS(ix, y - 1, w, h) || map[(y - 1) * w + ix] != TWALL)
                return -1;
        }
    }

    for (ix = x; ix > x - 5; --ix)
        map[y * w + ix] = TFLOOR;

    mgenerator_add_node(mgen, x - 5, y);
    mgenerator_add_node(mgen, x + 1, y);
    mgenerator_add_node(mgen, x - rng_under(&rng, 5), y - 1);
    mgenerator_add_node(mgen, x - rng_under(&rng, 5), y + 1);

    return 0;
}

int up_hall_5(mgenerator_type *mgen, int *map, int w, int h, int x, int y)
{
    int iy;

    for (iy = y; iy > y - 5; --iy) {
        if (!IN_BOUNDS(x, iy, w, h) || map[iy * w + x]  != TWALL)
            return -1;

        if (iy < y && iy - 1 > y - 5) {
            if (!IN_BOUNDS(x + 1, iy, w, h) || map[iy * w + (x + 1)] != TWALL)
                return -1;
            if (!IN_BOUNDS(x - 1, iy, w, h) || map[iy * w + (x - 1)] != TWALL)
                return -1;
        }
    }

    for (iy = y; iy > y - 5; --iy)
        map[iy * w + x] = TFLOOR;

    mgenerator_add_node(mgen, x, y - 5);
    mgenerator_add_node(mgen, x, y + 1);
    mgenerator_add_node(mgen, x + 1, y - rng_under(&rng, 5));
    mgenerator_add_node(mgen, x - 1, y - rng_under(&rng, 5));

    return 0;
}

int down_hall_5(mgenerator_type *mgen, int *map, int w, int h, int x, int y)
{
    int iy;

    for (iy = y; iy < y + 5; ++iy) {
        if (!IN_BOUNDS(x, iy, w, h) || map[iy * w + x] != TWALL)
            return -1;

        if (iy > y && iy + 1 < y + 5) {
            if (!IN_BOUNDS(x + 1, iy, w, h) || map[iy * w + (x + 1)]  != TWALL)
                return -1;
            if (!IN_BOUNDS(x - 1, iy, w, h) || map[iy * w + (x - 1)] != TWALL)
                return -1;
        }
    }

    for (iy = y; iy < y + 5; ++iy)
        map[iy * w + x] = TFLOOR;

    mgenerator_add_node(mgen, x, y + 5);
    mgenerator_add_node(mgen, x, y - 1);
    mgenerator_add_node(mgen, x + 1, y + rng_under(&rng, 5));
    mgenerator_add_node(mgen, x - 1, y + rng_under(&rng, 5));

    return 0;
}

int room_XxX(mgenerator_type *mgen, int *map, int w, int h, int x, int y, int size)
{
    int ix, iy;
    int size2 = size / 2;
    int size2p1 = size2 + 1;

    for (ix = x - size2p1; ix <= x + size2p1; ++ix) {
        for (iy = y - size2p1; iy <= y + size2p1; ++iy) {
            if (ix == x || iy == y)
                continue;
            if (!IN_BOUNDS(ix, iy, w, h) || map[iy * w + ix] != TWALL)
                return -1;
        }
    }

    for (ix = x - size2; ix <= x + size2; ++ix)
        for (iy = y - size2; iy <= y + size2; ++iy)
            map[iy * w + ix] = TFLOOR;

    if (rng_under(&rng, 100) < 40) {
        map[(y - size2) * w + (x - size2)] = TTABLE;
    }

    if (rng_under(&rng, 100) < 40) {
        map[(y - size2) * w + (x + size2)] = TTABLE;
    }

    if (rng_under(&rng, 100) < 40) {
        map[(y + size2) * w + (x - size2)] = TTABLE;
    }

    if (rng_under(&rng, 100) < 40) {
        map[(y + size2) * w + (x + size2)] = TTABLE;
    }

    mgenerator_add_node(mgen, x, y + size2p1);
    mgenerator_add_node(mgen, x, y - size2p1);
    mgenerator_add_node(mgen, x + size2p1, y);
    mgenerator_add_node(mgen, x - size2p1, y);

    return 0;
}


int diamond_7(mgenerator_type *mgen, int *map, int w, int h, int x, int y)
{
    int border_delta[12][2] = {
        {-3, -1},
        {-2, -1},
        {-1, -2},
        {-2, -1},
        {-1, -2},
        {-1, -3},

        { 3, -1},
        { 2, -1},
        { 1, -2},
        { 2, -1},
        { 1, -2},
        { 1, -3},
    };
    int delta[16][2] = {
        {-3,  0},
        {-2,  0},
        {-2, -1},
        {-1,  0},
        {-1, -1},
        {-1, -2},
        { 0,  0},
        { 0, -1},
        { 0, -2},
        { 0, -3},

        { 1,  0},
        { 1, -1},
        { 1, -2},
        { 2,  0},
        { 2, -1},
        { 3,  0},
    };
    int i, tx, ty;

    for (i = 0; i < 12; ++i) {
        tx = x + border_delta[i][0];
        ty = y + border_delta[i][1];
        if (!IN_BOUNDS(tx, ty, w, h) || map[ty * w + tx] != TWALL)
            return -1;
        ty = y + border_delta[i][1] * -1;
        if (!IN_BOUNDS(tx, ty, w, h) || map[ty * w + tx] != TWALL)
            return -1;
    }

    for (i = 0; i < 16; ++i) {
        tx = x + delta[i][0];
        ty = y + delta[i][1];
        map[ty * w + tx] = TFLOOR;
        ty = y + delta[i][1] * -1;
        map[ty * w + tx] = TFLOOR;
    }

    mgenerator_add_node(mgen, x, y + 4);
    mgenerator_add_node(mgen, x, y - 4);
    mgenerator_add_node(mgen, x + 4, y);
    mgenerator_add_node(mgen, x - 4, y);

    return 0;
}

int room_7x7(mgenerator_type *mgen, int *map, int w, int h, int x, int y)
{
    return room_XxX(mgen, map, w, h, x, y, 7);
}

int room_5x5(mgenerator_type *mgen, int *map, int w, int h, int x, int y)
{
    return room_XxX(mgen, map, w, h, x, y, 5);
}

int room_3x3(mgenerator_type *mgen, int *map, int w, int h, int x, int y)
{
    return room_XxX(mgen, map, w, h, x, y, 3);
}

int treasure_3x3(mgenerator_type *mgen, int *map, int w, int h, int x, int y)
{
    if (room_XxX(mgen, map, w, h, x, y, 3) == -1)
        return -1;

    map[(y - 1) * w + (x - 1)] = TCHEST;
    map[(y - 1) * w + (x + 1)] = TCHEST;
    map[(y + 1) * w + (x - 1)] = TCHEST;
    map[(y + 1) * w + (x + 1)] = TCHEST;

    return 0;
}
