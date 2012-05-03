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
#include "queue.h"

#include <stdlib.h>

struct queue_node {
    struct queue_node  *next;
    void               *mem;
};



int queue_enqueue(struct queue *q, void *mem)
{
    struct queue_node *node = malloc(sizeof *node);
    if (!node)
        return -1;

    if (!q->bot)
        q->bot = node;
    if (q->top)
        q->top->next = node;
    node->next = NULL;
    node->mem = mem;
    q->top = node;

    return 1;
}



void queue_dequeue(struct queue *q, void (*freer) (void *v))
{
    struct queue_node *tofree = q->bot;
    q->bot = q->bot->next;
    if (!q->bot)
        q->top = NULL;
    if (freer)
        freer(tofree->mem);
    free(tofree);
}



void queue_peek(struct queue *q, void **mem)
{
    *mem = q->bot->mem;
}
