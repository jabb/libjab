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
#include <stdlib.h>
#include "darray.h"
#include "mgenerator.h"
#include "random.h"

struct point {
    int x, y;
};

struct plan_and_weight {
    mgenerator_plan *plan;
    int weight;
};

static struct plan_and_weight *select_plan(struct darray *plans)
{
    unsigned i, r, cap = 0;
    struct plan_and_weight *tmp;

    for (i = 0; i < plans->length; ++i) {
        darray_at(plans, (void **)&tmp, i);
        cap += tmp->weight;
    }

    r = mtrandom() * cap;

    while (r < cap) {
        darray_at(plans, (void **)&tmp, --i);
        cap -= tmp->weight;
    }

    return tmp;
}

int mgenerator_open(struct mgenerator *mgen)
{
    mgen->nodes = malloc(sizeof *mgen->nodes);
    mgen->plans = malloc(sizeof *mgen->plans);

    if (!mgen->nodes || !mgen->plans) {
        free(mgen->nodes);
        free(mgen->plans);
        return -1;
    }

    if (darray_open(mgen->nodes, 3) == -1) {
        free(mgen->nodes);
        free(mgen->plans);
        return -1;
    }
    if (darray_open(mgen->plans, 3) == -1) {
        darray_close(mgen->nodes, free);
        free(mgen->nodes);
        free(mgen->plans);
        return -1;
    }

    return 0;
}

void mgenerator_close(struct mgenerator *mgen)
{
    darray_close(mgen->nodes, free);
    darray_close(mgen->plans, free);
    free(mgen->nodes);
    free(mgen->plans);
}

int mgenerator_add_node(struct mgenerator *mgen, int x, int y)
{
    struct point *p = malloc(sizeof *p);
    if (!p)
        return -1;
    p->x = x;
    p->y = y;

    if (darray_push_back(mgen->nodes, (void *)p) == -1) {
        free(p);
        return -1;
    }

    return 0;
}

int mgenerator_add_plan(struct mgenerator *mgen, mgenerator_plan *plan, int weight)
{
    struct plan_and_weight *p = malloc(sizeof *p);
    if (!p)
        return -1;

    p->plan = plan;
    p->weight = weight;

    if (darray_push_back(mgen->plans, (void *)p) == -1) {
        free(p);
        return -1;
    }

    return 0;
}

int mgenerator_generate(struct mgenerator *mgen, int *map, int w, int h, int lim)
{
    const int MAX_PLAN_TRIES = 10;
    int r, i, successes = 0;
    struct point *p;
    struct plan_and_weight *selected;
    struct plan_and_weight *prev = NULL;

    while (mgen->nodes->length) {
        r = mtrandom() * mgen->nodes->length;
        darray_at(mgen->nodes, (void **)&p, r);

        for (i = 0; i < MAX_PLAN_TRIES; ++i) {
            selected = select_plan(mgen->plans);
            while (prev && selected == prev)
                selected = select_plan(mgen->plans);
            prev = selected;

            if (selected->plan(mgen, map, w, h, p->x, p->y) == 0) {
                successes++;
                break;
            }
        }

        darray_remove(mgen->nodes, free, r);

        if (successes >= lim)
            break;
    }

    while (mgen->nodes->length)
        darray_remove(mgen->nodes, free, mgen->nodes->length - 1);

    return successes;
}

