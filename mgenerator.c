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

#define MAKE_POINT(x, y) ((long)(x) << 16 | (y))
#define X(p) ((p) >> 16 & 0xffff)
#define Y(p) ((p) & 0xffff)

struct param {
    int (*create) (int *, int, int, int, int);
    int weight;
};

static struct param *select_param(struct darray *params)
{
    unsigned i, r, cap = 0;
    struct param *tmp;

    for (i = 0; i < params->length; ++i) {
        darray_at(params, (void **)&tmp, i);
        cap += tmp->weight;
    }

    r = mtrandom() * cap;

    while (r < cap) {
        darray_at(params, (void **)&tmp, --i);
        cap -= tmp->weight;
    }

    return tmp;
}

int mgenerator_open(struct mgenerator *mgen)
{
    mgen->nodes = malloc(sizeof *mgen->nodes);
    mgen->params = malloc(sizeof *mgen->params);

    if (!mgen->nodes || !mgen->params) {
        free(mgen->nodes);
        free(mgen->params);
        return -1;
    }

    if (darray_open(mgen->nodes, 3) == -1) {
        free(mgen->nodes);
        free(mgen->params);
        return -1;
    }
    if (darray_open(mgen->params, 3) == -1) {
        darray_close(mgen->nodes, free);
        free(mgen->nodes);
        free(mgen->params);
        return -1;
    }

    return 0;
}

void mgenerator_close(struct mgenerator *mgen)
{
    darray_close(mgen->nodes, NULL);
    darray_close(mgen->params, free);
}

int mgenerator_add_node(struct mgenerator *mgen, int x, int y)
{
    return darray_push_back(mgen->nodes, (void *)MAKE_POINT(x, y));
}

int mgenerator_add_param(struct mgenerator *mgen, int (*create) (int *, int, int, int, int), int weight)
{
    struct param *p = malloc(sizeof *p);
    if (!p)
        return -1;

    p->create = create;
    p->weight = weight;

    if (darray_push_back(mgen->params, (void *)p) == -1) {
        free(p);
        return -1;
    }

    return 0;
}

int mgenerator_generate(struct mgenerator *mgen, int *map, int w, int h)
{
    unsigned r, p, successes = 0;
    struct param *selected;

    while (mgen->nodes->length) {
        r = mtrandom() * mgen->nodes->length;
        darray_at(mgen->nodes, (void **)&p, r);
        darray_remove(mgen->nodes, NULL, r);

        selected = select_param(mgen->params);
        if (selected->create(map, w, h, X(p), Y(p)) == 0)
            successes++;
    }

    return successes;
}

