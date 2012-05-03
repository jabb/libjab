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
#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************\
Common
\******************************************************************************/

typedef void free_func(void *);
/* Returns 0 on equality,
 *        <0 when the first arg is less than the second,
 *        >0 when the first arg is greater than the first.
 */
typedef int comp_func(void *, void *);

void *box_long(long l);
void *box_double(double d);
void *box_string(const char *s);

int comp_long(void *a, void *b);
int comp_double(void *a, void *b);
int comp_string(void *a, void *b);

/******************************************************************************\
Stack
\******************************************************************************/

struct stack_node;
struct stack {
    struct stack_node *top;
};

void stack_init(struct stack *s);
void stack_uninit(struct stack *s, free_func *freer);
int stack_push(struct stack *s, void *mem);
void stack_pop(struct stack *s, free_func *freer);
void stack_peek(struct stack *s, void **mem);

/******************************************************************************\
Queue
\******************************************************************************/

struct queue_node;
struct queue {
    struct queue_node *bot;
    struct queue_node *top;
};

void queue_init(struct queue *q);
void queue_uninit(struct queue *q, free_func *freer);
int queue_enqueue(struct queue *q, void *mem);
void queue_dequeue(struct queue *q, free_func *freer);
void queue_peek(struct queue *q, void **mem);

/******************************************************************************\
Dynamic Array
\******************************************************************************/

struct darray {
    void **mem;
    unsigned length;
    unsigned allocd;
};

int darray_init(struct darray *arr, unsigned size);
void darray_uninit(struct darray *arr, free_func *freer);
int darray_push_back(struct darray *arr, void *mem);
void darray_pop_back(struct darray *arr, free_func *freer);
int darray_insert(struct darray *arr, int pos, void *mem);
void darray_remove(struct darray *arr, free_func *freer, int pos);
void darray_at(struct darray *arr, int pos, void **mem);

/******************************************************************************\
Priority Queue
\******************************************************************************/

struct pqueue {
    void **mem;
    unsigned length, allocd;
};

int pqueue_init(struct pqueue *pq, unsigned size);
void pqueue_uninit(struct pqueue *pq, free_func *freer);
/* DO NOT MIX MIN's AND MAX's */
int pqueue_push_min(struct pqueue *pq, void *mem, comp_func *compare);
void pqueue_pop_min(struct pqueue *pq, free_func *freer, comp_func *compare);
int pqueue_push_max(struct pqueue *pq, void *mem, comp_func *compare);
void pqueue_pop_max(struct pqueue *pq, free_func *freer, comp_func *compare);
void pqueue_peek(struct pqueue *pq, void **mem);

/******************************************************************************\
Hash
\******************************************************************************/

struct hash_bucket;

struct hash {
    struct darray *buckets;
    unsigned max_size;
};

int hash_init(struct hash *hash, unsigned siz);
void hash_uninit(struct hash *hash, free_func *freer);
int hash_insert(struct hash *hash, const char *key, void *mem, free_func *freer);
int hash_exists(struct hash *hash, const char *key);
void hash_remove(struct hash *hash, const char *key, free_func *freer);
void hash_at(struct hash *hash, const char *key, void **mem);
void hash_keys(struct hash *hash, struct darray *arr);
void hash_values(struct hash *hash, struct darray *arr);

#ifdef __cplusplus
}
#endif

#endif