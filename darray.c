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

int darray_open(struct darray *arr, unsigned size)
{
    arr->mem = calloc(size, sizeof *arr->mem);
    if (!arr->mem)
        return -1;
    arr->length = 0;
    arr->allocd = size;
    return 0;
}

void darray_close(struct darray *arr, void (*freer) (void *))
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

void darray_pop_back(struct darray *arr, void (*freer) (void *))
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

    memmove(&arr->mem[pos + 1], &arr->mem[pos], sizeof *arr->mem * (arr->length - pos));
    arr->mem[pos] = mem;
    arr->length++;
    return 0;
}

void darray_remove(struct darray *arr, void (*freer) (void *), int pos)
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

