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
#ifndef HASH_H
#define HASH_H

#ifdef __cplusplus
extern "C" {
#endif

struct darray;
struct hash_bucket;

struct hash {
    struct darray *buckets;
    unsigned max_size;
};

int hash_open(struct hash *hash, unsigned siz);
void hash_close(struct hash *hash, void (*freer) (void *));
int hash_insert(struct hash *hash, const char *key, void *mem, void (*freer) (void *));
int hash_exists(struct hash *hash, const char *key);
void hash_remove(struct hash *hash, const char *key, void (*freer) (void *));
void hash_at(struct hash *hash, const char *key, void **mem);
void hash_keys(struct hash *hash, struct darray *arr);
void hash_values(struct hash *hash, struct darray *arr);

#ifdef __cplusplus
}
#endif

#endif

