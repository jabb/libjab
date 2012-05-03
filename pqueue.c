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
#include "pqueue.h"

#include <stdlib.h>



int pqueue_open(struct pqueue *pq, unsigned size)
{
    pq->mem = malloc(sizeof *pq->mem * size);
    if (!pq->mem)
        return -1;
    pq->size = 0;
    pq->allocd = size;
    return 0;
}



void pqueue_close(struct pqueue *pq, void (*freer) (void *))
{
    unsigned i;
    if (freer)
        for (i = 1; i < pq->size + 1; ++i)
            freer(pq->mem[i]);
    free(pq->mem);
}



int pqueue_push(struct pqueue *pq, void *mem, int (*comparitor) (void *, void *))
{
    int i;
    void **new_mem;

    if (pq->size + 1 >= pq->allocd) {
        new_mem = realloc(pq->mem, sizeof *pq->mem * pq->allocd * 2);
        if (!new_mem)
            return -1;
        pq->mem = new_mem;
        pq->allocd *= 2;
    }

    for (i = ++pq->size; i > 1 && comparitor(pq->mem[i / 2], mem) > 0; i /= 2)
        pq->mem[i] = pq->mem[i / 2];
    pq->mem[i] = mem;

    return 0;
}



void pqueue_pop(struct pqueue *pq, void (*freer) (void *), int (*comparitor) (void *, void *))
{
    unsigned i, child;
    void *tofree = pq->mem[1];
    void *last = pq->mem[pq->size--];
    if (freer)
        freer(tofree);

    for (i = 1; i * 2 <= pq->size; i = child) {
        child = i * 2;
        if (child != pq->size && comparitor(pq->mem[child], pq->mem[child + 1]) > 0)
            child++;

        if (comparitor(last, pq->mem[child]) > 0)
            pq->mem[i] = pq->mem[child];
        else
            break;
    }

    pq->mem[i] = last;
}



void pqueue_top(struct pqueue *pq, void **mem)
{
    *mem = pq->mem[1];
}

