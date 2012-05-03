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
#include "mgenerator.h"

#include <stdlib.h>

#include "random.h"

typedef struct {
    int x, y;
} point_struct;

typedef struct {
    mgenerator_plan *plan;
    int weight;
} plan_and_weight_struct;

extern rng_state rng;

static plan_and_weight_struct *select_plan(darray_type *plans)
{
    unsigned i, r, cap = 0;
    plan_and_weight_struct *tmp;

    for (i = 0; i < plans->length; ++i) {
        darray_at(plans, (void **)&tmp, i);
        cap += tmp->weight;
    }

    r = rng_under(&rng, cap);

    while (r < cap) {
        darray_at(plans, (void **)&tmp, --i);
        cap -= tmp->weight;
    }

    return tmp;
}

int mgenerator_open(mgenerator_type *mgen)
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

void mgenerator_close(mgenerator_type *mgen)
{
    darray_close(mgen->nodes, free);
    darray_close(mgen->plans, free);
    free(mgen->nodes);
    free(mgen->plans);
}

int mgenerator_add_node(mgenerator_type *mgen, int x, int y)
{
    point_struct *p = malloc(sizeof *p);
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

int mgenerator_add_plan(mgenerator_type *mgen, mgenerator_plan *plan, int weight)
{
    plan_and_weight_struct *p = malloc(sizeof *p);
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

int mgenerator_generate(mgenerator_type *mgen, int *map, int w, int h, int lim)
{
    int r, successes = 0, found;
    unsigned i;
    point_struct *p;
    plan_and_weight_struct *selected;
    plan_and_weight_struct *tried;
    darray_type tries;

    darray_open(&tries, 16);

    while (mgen->nodes->length) {
        r = rng_under(&rng, mgen->nodes->length);
        darray_at(mgen->nodes, (void **)&p, r);

        while (tries.length < mgen->plans->length) {
            selected = select_plan(mgen->plans);
            for (;;) {
                found = 0;
                for (i = 0; i < tries.length; ++i) {
                    darray_at(&tries, (void **)&tried, i);
                    if (tried == selected)
                        found = 1;
                }

                if (found)
                    selected = select_plan(mgen->plans);
                else
                    break;
            }

            darray_push_back(&tries, selected);

            if (selected->plan(mgen, map, w, h, p->x, p->y) == 0) {
                successes++;
                break;
            }
        }

        darray_remove(mgen->nodes, free, r);

        while (tries.length)
            darray_remove(&tries, NULL, tries.length - 1);

        if (successes >= lim)
            break;
    }

    darray_close(&tries, NULL);

    while (mgen->nodes->length)
        darray_remove(mgen->nodes, free, mgen->nodes->length - 1);

    return successes;
}

