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
#ifndef HASH_H
#define HASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "darray.h"

typedef struct hash_bucket hash_bucket;

typedef struct {
    darray_type    *buckets;
    unsigned        max_size;
} hash_type;

int hash_open(hash_type *hash, unsigned siz);
void hash_close(hash_type *hash, void (*freer) (void *));
int hash_insert(hash_type *hash, const char *key, void *mem, void (*freer) (void *));
int hash_exists(hash_type *hash, const char *key);
void hash_remove(hash_type *hash, const char *key, void (*freer) (void *));
void hash_at(hash_type *hash, const char *key, void **mem);
void hash_keys(hash_type *hash, darray_type *arr);
void hash_values(hash_type *hash, darray_type *arr);

#ifdef __cplusplus
}
#endif

#endif

