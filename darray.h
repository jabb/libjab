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
#ifndef DARRAY_H
#define DARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

struct darray {
    void **mem;
    unsigned length;
    unsigned allocd;
};

int darray_open(struct darray *arr, unsigned size);
void darray_close(struct darray *arr, void (*freer) (void *));
int darray_push_back(struct darray *arr, void *mem);
void darray_pop_back(struct darray *arr, void (*freer) (void *));
int darray_insert(struct darray *arr, void *mem, int pos);
void darray_remove(struct darray *arr, void (*freer) (void *), int pos);
void darray_at(struct darray *arr, void **mem, int pos);

#ifdef __cplusplus
}
#endif

#endif

