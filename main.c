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
            color = 0x111111;
        else
            color = 0xff0000;
        break;
    case TFLOOR:
        tile = AT_AUX_SYM_FLOOR;
        if (flags)
            color = 0x111111;
        else
            color = 0x777777;
        break;
    case TWALL:
        tile = AT_AUX_SYM_WALL;
        if (flags)
            color = 0x111111;
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
            if (1) { /*(can_see(px, py, x, y, 8, (int *)map, MWIDTH)) {*/
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

int line(int *map, int w, int h, int x0, int y0, int x1, int y1)
{
    int i;
    int path[128][2];

    if (!IN_BOUNDS(x0, y0, w, h) || !IN_BOUNDS(x1, y1, w, h))
        return -1;

    path_to(x0, y0, x1, y1, (int *)path, 128);

    for (i = 0; path[i][0] != -1; ++i) {
        if (!map[path[i][1] * w + path[i][0]])
            return -1;
    }

    for (i = 0; path[i][0] != -1; ++i) {
        map[path[i][1] * w + path[i][0]] = 0;
    }

    return 0;
}

struct mgenerator mgen;

int create_hall_5x1(int *map, int w, int h, int x, int y)
{
    int ends[4][2] = {
        {-5,  0},
        { 5,  0},
        { 0, -5},
        { 0,  5}
    };
    int tried[4] = {0, 0, 0, 0};
    int nodes[8][2] = {
        {-6,  0},
        { 1,  0},
        { 6,  0},
        {-1,  0},
        { 0, -6},
        { 0,  1},
        { 0,  6},
        { 0, -1}
    };
    int sx, sy, ex, ey, i, r;

    sx = x;
    sy = y;

    i = 4;
    while (i --> 0) {
        r = mtrandom() * 4;
        while (tried[r])
            r = mtrandom() * 4;
        tried[r] = 1;

        ex = x + ends[r][0];
        ey = y + ends[r][1];

        if (line(map, w, h, sx, sy, ex, ey) == 0) {
            if (IN_BOUNDS(x + nodes[r * 2][0], y + nodes[r * 2][1], MWIDTH, MHEIGHT))
                mgenerator_add_node(&mgen, x + nodes[r * 2][0], y + nodes[r * 2][1]);
            /*if (IN_BOUNDS(x + nodes[r * 2 + 1][0], y + nodes[r * 2 + 1][1], MWIDTH, MHEIGHT))
                mgenerator_add_node(&mgen, x + nodes[r * 2 + 1][0], y + nodes[r * 2 + 1][1]);*/
            return 0;
        }
    }

    return -1;
}

int create_hall_10x1(int *map, int w, int h, int x, int y)
{
    int ends[4][2] = {
        {-10,   0},
        { 10,   0},
        {  0, -10},
        {  0,  10}
    };
    int tried[4] = {0, 0, 0, 0};
    int nodes[8][2] = {
        {-11,   0},
        {  1,   0},
        { 11,   0},
        { -1,   0},
        {  0, -11},
        {  0,   1},
        {  0,  11},
        {  0,  -1}
    };
    int sx, sy, ex, ey, i, r;

    sx = x;
    sy = y;

    i = 4;
    while (i --> 0) {
        r = mtrandom() * 4;
        while (tried[r])
            r = mtrandom() * 4;
        tried[r] = 1;

        ex = x + ends[r][0];
        ey = y + ends[r][1];

        if (line(map, w, h, sx, sy, ex, ey) == 0) {
            if (IN_BOUNDS(x + nodes[r * 2][0], y + nodes[r * 2][1], MWIDTH, MHEIGHT))
                mgenerator_add_node(&mgen, x + nodes[r * 2][0], y + nodes[r * 2][1]);
            if (IN_BOUNDS(x + nodes[r * 2 + 1][0], y + nodes[r * 2 + 1][1], MWIDTH, MHEIGHT))
                mgenerator_add_node(&mgen, x + nodes[r * 2 + 1][0], y + nodes[r * 2 + 1][1]);
            break;
        }
    }

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
    mgenerator_add_param(&mgen, create_hall_5x1, 3);
    mgenerator_add_param(&mgen, create_hall_10x1, 1);

    mgenerator_add_node(&mgen, mtrandom() * MWIDTH, mtrandom() * MHEIGHT);
    while (mgenerator_generate(&mgen, (int *)map, MWIDTH, MHEIGHT) < 10) {
        for (x = 0; x < MWIDTH; ++x)
            for (y = 0; y < MHEIGHT; ++y)
                map[y][x] = 1;
        mgenerator_add_node(&mgen, mtrandom() * MWIDTH, mtrandom() * MHEIGHT);
    }

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


