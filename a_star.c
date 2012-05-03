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
#include <stdlib.h>
#include <string.h>

#include "a_star.h"
#include "pqueue.h"

struct a_star_node {
    struct a_star_node *parent;
    int closed;
    int x, y;
    int g, h;
};

static int pqueue_has(pqueue_type *pq, void *mem)
{
    unsigned i;
    for (i = 1; i < pq->size + 1; ++i)
        if (pq->mem[i] == mem)
            return 1;
    return 0;
}

static int heuristic(int x0, int y0, int x1, int y1)
{
    int dx = abs(x0 - x1);
    int dy = abs(y0 - y1);
    int diag = dx < dy ? dx : dy;
    int straight = dx + dy;
    return diag + (straight - 2 * diag);
}

static int a_star_node_cmp(void *a, void *b)
{
    return (((struct a_star_node *)a)->g + ((struct a_star_node *)a)->h) -
           (((struct a_star_node *)b)->g + ((struct a_star_node *)b)->h);
}

int a_star(int x0, int y0, int x1, int y1, int *map, int w, int h, int *path, int sz)
{
    static int dirs[8][3] = {
        {-1,  0, 10}, { 0, -1, 10}, { 0,  1, 10}, { 1,  0, 10},
        {-1, -1, 14}, {-1,  1, 14}, { 1, -1, 14}, { 1,  1, 14},
    };
    int ret = 1, i, nx, ny;
    struct a_star_node *nodes = NULL;
    struct a_star_node *ntmp = NULL, *head = NULL, *reversed = NULL;
    struct a_star_node *lowest = NULL, *neighbor = NULL;
    pqueue_type open;

    nodes = malloc(sizeof *nodes * w * h);
    if (!nodes) {
        ret = -1;
        goto failure;
    }
    memset(nodes, 0, sizeof *nodes * w * h);

    nodes[y0 * w + x0].x = x0;
    nodes[y0 * w + x0].y = y0;

    pqueue_open(&open, 256);
    pqueue_push(&open, &nodes[y0 * w + x0], a_star_node_cmp);

    while (open.size) {
        pqueue_top(&open, (void **)&lowest);
        pqueue_pop(&open, NULL, a_star_node_cmp);
        lowest->closed = 1;

        if (lowest->x == x1 && lowest->y == y1)
            break;

        for (i = 0; i < 8; ++i) {
            nx = lowest->x + dirs[i][0];
            ny = lowest->y + dirs[i][1];
            neighbor = &nodes[ny * w + nx];
            if (nx >= 0 && nx < w && ny >= 0 && ny < h && !map[ny * w + nx] && !neighbor->closed) {
                if (!pqueue_has(&open, neighbor)) {
                    neighbor->x = nx;
                    neighbor->y = ny;
                    neighbor->g = lowest->g + dirs[i][2];
                    neighbor->h = heuristic(nx, ny, x1, y1);
                    pqueue_push(&open, neighbor, a_star_node_cmp);
                    neighbor->parent = lowest;
                }
                else if (lowest->g + dirs[i][2] < neighbor->g) {
                    neighbor->g = lowest->g + dirs[i][2];
                    neighbor->parent = lowest;
                }
            }
        }
    }

    if (!nodes[y1 * w + x1].parent) {
        path[0] = -1;
        path[1] = -1;
        ret = 0;
        goto failure;
    }

    for (head = &nodes[y1 * w + x1]; head; ) {
        ntmp = head;
        head = head->parent;
        ntmp->parent = reversed;
        reversed = ntmp;
    }

    for (i = 0; reversed; reversed = reversed->parent) {
        path[i * 2 + 0] = reversed->x;
        path[i * 2 + 1] = reversed->y;
        if (++i >= sz - 1)
            break;
    }
    path[i * 2 + 0] = -1;
    path[i * 2 + 1] = -1;

failure:
    free(nodes);
    pqueue_close(&open, NULL);
    return ret;
}

