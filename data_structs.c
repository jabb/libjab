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
#include "data_structs.h"

#include <stdlib.h>
#include <string.h>

/******************************************************************************\
Common
\******************************************************************************/



void *box_long(long l)
{
    long *v = malloc(sizeof *v);
    if (!v)
        return NULL;
    *v = l;
    return v;
}



void *box_double(double d)
{
    double *v = malloc(sizeof *v);
    if (!v)
        return NULL;
    *v = d;
    return v;
}



void *box_string(const char *s)
{
    char *v = malloc(strlen(s) + 1);
    if (!v)
        return NULL;
    strcpy(v, s);
    return v;
}



int comp_double(void *a, void *b)
{
    double x = *(double *)a;
    double y = *(double *)b;
    return x < y ? -1 :
           y < x ?  1 :
           0;
}



int comp_long(void *a, void *b)
{
    long x = *(long *)a;
    long y = *(long *)b;
    return x - y;
}



int comp_string(void *a, void *b)
{
    return strcmp(a, b);
}

/******************************************************************************\
Stack
\******************************************************************************/



struct stack_node {
    struct stack_node  *below;
    void               *mem;
};



void stack_init(struct stack *s)
{
    s->top = NULL;
}



void stack_uninit(struct stack *s, free_func *freer)
{
    while (s->top)
        stack_pop(s, freer);
}



int stack_push(struct stack *s, void *mem)
{
    struct stack_node *node = malloc(sizeof *node);
    if (!node)
        return -1;

    node->below = s->top;
    node->mem = mem;
    s->top = node;

    return 0;
}



void stack_pop(struct stack *s, free_func *freer)
{
    struct stack_node *tofree = s->top;
    s->top = s->top->below;
    if (freer)
        freer(tofree->mem);
    free(tofree);
}



void stack_peek(struct stack *s, void **mem)
{
    *mem = s->top->mem;
}

/******************************************************************************\
Queue
\******************************************************************************/



struct queue_node {
    struct queue_node  *next;
    void               *mem;
};



void queue_init(struct queue *q)
{
    q->top = NULL;
    q->bot = NULL;
}



void queue_uninit(struct queue *q, free_func *freer)
{
    while (q->top)
        queue_dequeue(q, freer);
}



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

    return 0;
}



void queue_dequeue(struct queue *q, free_func *freer)
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

/******************************************************************************\
Dynamic Array
\******************************************************************************/



int darray_init(struct darray *arr, unsigned size)
{
    arr->mem = calloc(size, sizeof *arr->mem);
    if (!arr->mem)
        return -1;
    arr->length = 0;
    arr->allocd = size;
    return 0;
}



void darray_uninit(struct darray *arr, free_func *freer)
{
    while (arr->length)
        darray_pop_back(arr, freer);
    free(arr->mem);
}



int darray_push_back(struct darray *arr, void *mem)
{
    void *tmp;

    if (arr->length >= arr->allocd) {
        tmp = realloc(arr->mem, sizeof *arr->mem * arr->allocd * 2);
        if (!tmp)
            return -1;
        arr->mem = tmp;
        arr->allocd *= 2;
    }

    arr->mem[arr->length++] = mem;
    return 0;
}



void darray_pop_back(struct darray *arr, free_func *freer)
{
    if (freer)
        freer(arr->mem[--arr->length]);
    else
        arr->length--;
}



int darray_insert(struct darray *arr, void *mem, int pos)
{
    void *tmp;

    if (arr->length >= arr->allocd) {
        tmp = realloc(arr->mem, sizeof *arr->mem * arr->allocd * 2);
        if (!tmp)
            return -1;
        arr->mem = tmp;
        arr->allocd *= 2;
    }

    memmove(&arr->mem[pos + 1],
            &arr->mem[pos],
            sizeof *arr->mem * (arr->length - pos));
    arr->mem[pos] = mem;
    arr->length++;
    return 0;
}



void darray_remove(struct darray *arr, free_func *freer, int pos)
{
    if (freer)
        freer(arr->mem[pos]);

    arr->length--;
    memmove(&arr->mem[pos], &arr->mem[pos + 1], sizeof *arr->mem * (arr->length - pos));
}



void darray_at(struct darray *arr, void **mem, int pos)
{
    *mem = arr->mem[pos];
}

/******************************************************************************\
Priority Queue
\******************************************************************************/



int pqueue_init(struct pqueue *pq, unsigned size)
{
    pq->mem = malloc(sizeof *pq->mem * size);
    if (!pq->mem)
        return -1;
    pq->length = 0;
    pq->allocd = size;
    return 0;
}



void pqueue_uninit(struct pqueue *pq, free_func *freer)
{
    unsigned i;
    if (freer)
        for (i = 1; i < pq->length + 1; ++i)
            freer(pq->mem[i]);
    free(pq->mem);
}



int pqueue_push(struct pqueue *pq, void *mem, comp_func *compare)
{
    int i;
    void **new_mem;

    if (pq->length + 1 >= pq->allocd) {
        new_mem = realloc(pq->mem, sizeof *pq->mem * pq->allocd * 2);
        if (!new_mem)
            return -1;
        pq->mem = new_mem;
        pq->allocd *= 2;
    }

    for (i = ++pq->length; i > 1 && compare(pq->mem[i / 2], mem) > 0; i /= 2)
        pq->mem[i] = pq->mem[i / 2];
    pq->mem[i] = mem;

    return 0;
}



void pqueue_pop(struct pqueue *pq, free_func *freer, comp_func *compare)
{
    unsigned i, child;
    void *tofree = pq->mem[1];
    void *last = pq->mem[pq->length--];
    if (freer)
        freer(tofree);

    for (i = 1; i * 2 <= pq->length; i = child) {
        child = i * 2;
        if (child != pq->length && compare(pq->mem[child], pq->mem[child + 1]) > 0)
            child++;

        if (compare(last, pq->mem[child]) > 0)
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



/******************************************************************************\
Priority Queue
\******************************************************************************/



static unsigned hashstr(const char *str, unsigned size)
{
    unsigned h = 0;
    size_t i, n = strlen(str);

    for (i = 0; i < n; ++i) {
        h *= 31;
        h += str[i];
    }

    return h % size;
}



static char *string_dup(const char *str)
{
    char *dup = malloc(strlen(str) + 1);
    strcpy(dup, str);
    return dup;
}



struct hash_bucket {
    char *key;
    void *mem;
};



int hash_init(struct hash *hash, unsigned siz)
{
    unsigned i;
    hash->buckets = calloc(siz, sizeof *hash->buckets);
    if (!hash->buckets)
        return -1;
    hash->max_size = siz;

    for (i = 0; i < siz; ++i)
        darray_init(&hash->buckets[i], 3);

    return 0;
}



void hash_uninit(struct hash *hash, free_func *freer)
{
    unsigned i, j;
    struct hash_bucket *iter;

    for (i = 0; i < hash->max_size; ++i) {
        for (j = 0; j < hash->buckets[i].length; ++j) {
            darray_at(&hash->buckets[i], (void **)&iter, j);
            free(iter->key);
            if (freer)
                freer(iter->mem);
        }
        darray_uninit(&hash->buckets[i], free);
    }

    free(hash->buckets);
}



int hash_insert(struct hash *hash, const char *key, void *mem, free_func *freer)
{
    unsigned i, h;
    struct hash_bucket *bucket = malloc(sizeof *bucket), *iter;
    if (!bucket)
        return -1;

    bucket->key = string_dup(key);
    if (!bucket->key) {
        free(bucket);
        return -1;
    }
    bucket->mem = mem;

    h = hashstr(key, hash->max_size);

    for (i = 0; i < hash->buckets[h].length; ++i) {
        darray_at(&hash->buckets[h], (void **)&iter, i);
        if (!strcmp(iter->key, bucket->key)) {
            if (freer)
                freer(hash->buckets[h].mem[i]);
            hash->buckets[h].mem[i] = bucket;
            return 0;
        }
    }

    darray_push_back(&hash->buckets[h], bucket);

    return 0;
}



int hash_exists(struct hash *hash, const char *key)
{
    unsigned i, h;
    struct hash_bucket *iter;

    h = hashstr(key, hash->max_size);

    for (i = 0; i < hash->buckets[h].length; ++i) {
        darray_at(&hash->buckets[h], (void **)&iter, i);
        if (!strcmp(iter->key, key)) {
            return 1;
        }
    }
    return 0;
}



void hash_remove(struct hash *hash, const char *key, free_func *freer)
{
    unsigned i, h;
    struct hash_bucket *iter;

    h = hashstr(key, hash->max_size);

    for (i = 0; i < hash->buckets[h].length; ++i) {
        darray_at(&hash->buckets[h], (void **)&iter, i);
        if (!strcmp(iter->key, key)) {
            darray_remove(&hash->buckets[h], freer, i);
            break;
        }
    }
}



void hash_at(struct hash *hash, const char *key, void **mem)
{
    unsigned i, h;
    struct hash_bucket *iter;

    h = hashstr(key, hash->max_size);

    for (i = 0; i < hash->buckets[h].length; ++i) {
        darray_at(&hash->buckets[h], (void **)&iter, i);
        if (!strcmp(iter->key, key)) {
            *mem = iter->mem;
            return;
        }
    }

    *mem = NULL;
}



void hash_keys(struct hash *hash, struct darray *arr)
{
    unsigned i, j;
    struct hash_bucket *iter;

    for (i = 0; i < hash->max_size; ++i) {
        for (j = 0; j < hash->buckets[i].length; ++j) {
            darray_at(&hash->buckets[i], (void **)&iter, j);
            darray_push_back(arr, iter->key);
        }
    }
}



void hash_values(struct hash *hash, struct darray *arr)
{
    unsigned i, j;
    struct hash_bucket *iter;

    for (i = 0; i < hash->max_size; ++i) {
        for (j = 0; j < hash->buckets[i].length; ++j) {
            darray_at(&hash->buckets[i], (void **)&iter, j);
            darray_push_back(arr, iter->mem);
        }
    }
}
