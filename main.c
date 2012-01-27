/* Copyright (c) 2009, Michael Patraw
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
#include <time.h>
#include "at.h"
#include "at_aux.h"
#include "darray.h"
#include "can_see.h"
#include "mgenerator.h"
#include "random.h"

#define MWIDTH  40
#define MHEIGHT 20

#define IN_BOUNDS(x, y, w, h) ((x) >= 0 && (x) < (w) && (y) >= 0 && y < (h))

int seen_map[MHEIGHT][MWIDTH] = {{0}};
int map[MHEIGHT][MWIDTH] = {{0}};

int px = 0, py = 0;

enum {
    TPLAYER,
    TFLOOR,
    TWALL
};

static void output_tile(int x, int y, int tile, int flags)
{
    int color;

    at_set_font((unsigned *)at_aux_sym, AT_AUX_FWIDTH, AT_AUX_FHEIGHT);

    x *= AT_AUX_FWIDTH;
    y *= AT_AUX_FHEIGHT;

    switch (tile) {
    case TPLAYER:
        tile = AT_AUX_SYM_HUMAN;
        if (flags)
            color = 0x222222;
        else
            color = 0xff0000;
        break;
    case TFLOOR:
        tile = AT_AUX_SYM_FLOOR;
        if (flags)
            color = 0x222222;
        else
            color = 0x777777;
        break;
    case TWALL:
        tile = AT_AUX_SYM_WALL;
        if (flags)
            color = 0x222222;
        else
            color = 0x773300;
        break;
    default:
        tile = 0;
        color = 0;
        break;
    }

    at_plot_glyph(x, y, tile, color);

    at_set_font(NULL, 0, 0);
}

void pmove(int dx, int dy)
{
    if (IN_BOUNDS(px + dx, py + dy, MWIDTH, MHEIGHT) && !map[py + dy][px + dx]) {
        px += dx;
        py += dy;
    }
}

void input(void)
{
    if (at_peek('h'))
        pmove(-1, 0);
    else if (at_peek('j'))
        pmove(0, 1);
    else if (at_peek('k'))
        pmove(0, -1);
    else if (at_peek('l'))
        pmove(1, 0);
    else if (at_peek('y'))
        pmove(-1, -1);
    else if (at_peek('u'))
        pmove(1, -1);
    else if (at_peek('b'))
        pmove(-1, 1);
    else if (at_peek('n'))
        pmove(1, 1);
}

void output(void)
{
    int x, y;


    for (y = 0; y < MHEIGHT; ++y) {
        for (x = 0; x < MWIDTH; ++x) {
            if (1) {/*(can_see(px, py, x, y, 8, (int *)map, MWIDTH)) {*/
                seen_map[y][x] = 1;
                output_tile(x, y, map[y][x] ? TWALL : TFLOOR, 0);
            }
            else if (seen_map[y][x]) {
                output_tile(x, y, map[y][x] ? TWALL : TFLOOR, 1);
            }
        }
    }

    output_tile(px, py, TPLAYER, 0);
}

struct mgenerator mgen;

int create_left_hall_5(int *map, int w, int h, int x, int y)
{
    int ix;

    for (ix = x; ix < x + 5; ++ix) {
        if (!IN_BOUNDS(ix, y, w, h) || !map[y * w + ix])
            return -1;

        if (ix > x && ix + 1 < x + 5) {
            if (!IN_BOUNDS(ix, y + 1, w, h) || !map[(y + 1) * w + ix])
                return -1;
            if (!IN_BOUNDS(ix, y - 1, w, h) || !map[(y - 1) * w + ix])
                return -1;
        }
    }

    for (ix = x; ix < x + 5; ++ix)
        map[y * w + ix] = 0;

    mgenerator_add_node(&mgen, x - 1, y);
    mgenerator_add_node(&mgen, x + 5, y);
    mgenerator_add_node(&mgen, x + mtrandom() * 5, y - 1);
    mgenerator_add_node(&mgen, x + mtrandom() * 5, y + 1);

    return 0;
}

int create_right_hall_5(int *map, int w, int h, int x, int y)
{
    int ix;

    for (ix = x; ix > x - 5; --ix) {
        if (!IN_BOUNDS(ix, y, w, h) || !map[y * w + ix])
            return -1;

        if (ix < x && ix - 1 > x - 5) {
            if (!IN_BOUNDS(ix, y + 1, w, h) || !map[(y + 1) * w + ix])
                return -1;
            if (!IN_BOUNDS(ix, y - 1, w, h) || !map[(y - 1) * w + ix])
                return -1;
        }
    }

    for (ix = x; ix > x - 5; --ix)
        map[y * w + ix] = 0;

    mgenerator_add_node(&mgen, x - 5, y);
    mgenerator_add_node(&mgen, x + 1, y);
    mgenerator_add_node(&mgen, x - mtrandom() * 5, y - 1);
    mgenerator_add_node(&mgen, x - mtrandom() * 5, y + 1);

    return 0;
}

int create_up_hall_5(int *map, int w, int h, int x, int y)
{
    int iy;

    for (iy = y; iy > y - 5; --iy) {
        if (!IN_BOUNDS(x, iy, w, h) || !map[iy * w + x])
            return -1;

        if (iy < y && iy - 1 > y - 5) {
            if (!IN_BOUNDS(x + 1, iy, w, h) || !map[iy * w + (x + 1)])
                return -1;
            if (!IN_BOUNDS(x - 1, iy, w, h) || !map[iy * w + (x - 1)])
                return -1;
        }
    }

    for (iy = y; iy > y - 5; --iy)
        map[iy * w + x] = 0;

    mgenerator_add_node(&mgen, x, y - 5);
    mgenerator_add_node(&mgen, x, y + 1);
    mgenerator_add_node(&mgen, x + 1, y - mtrandom() * 5);
    mgenerator_add_node(&mgen, x - 1, y - mtrandom() * 5);

    return 0;
}

int create_down_hall_5(int *map, int w, int h, int x, int y)
{
    int iy;

    for (iy = y; iy < y + 5; ++iy) {
        if (!IN_BOUNDS(x, iy, w, h) || !map[iy * w + x])
            return -1;

        if (iy > y && iy + 1 < y + 5) {
            if (!IN_BOUNDS(x + 1, iy, w, h) || !map[iy * w + (x + 1)])
                return -1;
            if (!IN_BOUNDS(x - 1, iy, w, h) || !map[iy * w + (x - 1)])
                return -1;
        }
    }

    for (iy = y; iy < y + 5; ++iy)
        map[iy * w + x] = 0;

    mgenerator_add_node(&mgen, x, y + 5);
    mgenerator_add_node(&mgen, x, y - 1);
    mgenerator_add_node(&mgen, x + 1, y + mtrandom() * 5);
    mgenerator_add_node(&mgen, x - 1, y + mtrandom() * 5);

    return 0;
}

int create_room_7x7(int *map, int w, int h, int x, int y)
{
    int ix, iy;


    for (ix = x - 3; ix <= x + 3; ++ix) {
        for (iy = y - 3; iy <= y + 3; ++iy) {
            if (!IN_BOUNDS(ix, iy, w, h))
                return -1;
            if (ix == x || iy == y)
                continue;
            if (!map[iy * w + ix])
                return -1;
        }
    }

    for (ix = x - 3; ix <= x + 3; ++ix) {
        for (iy = y - 3; iy <= y + 3; ++iy) {
            map[iy * w + ix] = 0;
        }
    }

    mgenerator_add_node(&mgen, x, y + 4);
    mgenerator_add_node(&mgen, x, y - 4);
    mgenerator_add_node(&mgen, x + 4, y);
    mgenerator_add_node(&mgen, x - 4, y);

    return 0;
}

int main(int argc, char *argv[])
{
    unsigned x, y;
    (void)argc;
    (void)argv;

    for (x = 0; x < MWIDTH; ++x)
        for (y = 0; y < MHEIGHT; ++y)
            map[y][x] = 1;

    mtseed(time(NULL));
    mgenerator_open(&mgen);
    mgenerator_add_plan(&mgen, create_left_hall_5, 1);
    mgenerator_add_plan(&mgen, create_right_hall_5, 1);
    mgenerator_add_plan(&mgen, create_up_hall_5, 1);
    mgenerator_add_plan(&mgen, create_down_hall_5, 1);
    mgenerator_add_plan(&mgen, create_room_7x7, 1);

    mgenerator_add_node(&mgen, mtrandom() * MWIDTH, mtrandom() * MHEIGHT);
    mgenerator_generate(&mgen, (int *)map, MWIDTH, MHEIGHT);

    for (x = 0; x < MWIDTH; ++x)
        for (y = 0; y < MHEIGHT; ++y)
            if (!map[y][x]) {
                px = x, py = y;
                break;
            }

    at_open(MWIDTH * AT_FWIDTH, MHEIGHT * AT_FHEIGHT, "at", 3, 3);

    while (at_flush()) {
        if (at_peek('Q'))
            break;
        input();
        at_clear(0);
        output();
    }

    mgenerator_close(&mgen);
    at_close();
    return 0;
}
