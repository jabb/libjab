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

struct dlist_node {
    struct dlist_node *prev, *next;
    void *mem;
};

struct dlist {
    struct dlist_node *head;
    struct dlist_node *tail;
    int length;
};

int dlist_push_front(struct dlist *dl, void *mem)
{
    struct dlist_node *n = malloc(sizeof *n);
    if (!n)
        return -1;
    n->mem = mem;

    if (!dl->head) {
        dl->head = n;
        dl->tail = n;

        dl->head->next = dl->tail;
        dl->head->prev = NULL;
        dl->tail->next = NULL;
        dl->tail->prev = dl->head;
    }
    else if (dl->head == dl->tail) {

    }
    else {

    }

    return 0;
}

void dlist_pop_front(struct dlist *dl, void (*freer) (void *))
{
    (void)dl;
    (void)freer;
}

int dlist_push_back(struct dlist *dl, void *mem)
{
    (void)dl;
    (void)mem;
    return 0;
}

void dlist_pop_back(struct dlist *dl, void (*freer) (void *))
{
    (void)dl;
    (void)freer;
}

void dlist_front(struct dlist *dl, void **mem)
{
    *mem = dl->head->mem;
}

void dlist_back(struct dlist *dl, void **mem)
{
    *mem = dl->tail->mem;
}
