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
#include "darray.h"
#include "hash.h"

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

int hash_open(struct hash *hash, unsigned siz)
{
    unsigned i;
    hash->buckets = calloc(siz, sizeof *hash->buckets);
    if (!hash->buckets)
        return -1;
    hash->max_size = siz;

    for (i = 0; i < siz; ++i)
        darray_open(&hash->buckets[i], 3);

    return 0;
}

void hash_close(struct hash *hash, void (*freer) (void *))
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
        darray_close(&hash->buckets[i], free);
    }

    free(hash->buckets);
}

int hash_insert(struct hash *hash, const char *key, void *mem, void (*freer) (void *))
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

void hash_remove(struct hash *hash, const char *key, void (*freer) (void *))
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

