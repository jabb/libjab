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
#ifndef RANDOM_H
#define RANDOM_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Slow. Period = 2^19937 - 1. */
enum {MT19937_K = 624};

struct mt19937_state {
    uint32_t    mt[MT19937_K];
    int32_t     mti;
};

/* Fast. Period = 2^127 - 1. */
enum {TINYMT_K = 4};

struct tinymt_state {
    uint32_t status[TINYMT_K];
    uint32_t mat1;
    uint32_t mat2;
    uint32_t tmat;
};

/* Very Fast. Period = 2^128 - 1. */
enum {XOR128_K = 4};

struct xor128_state {
    uint32_t q[XOR128_K];
};

/* Fast. Period ~= 2^131104. */
enum {CMWC_K = 4096};

struct cmwc_state {
    uint32_t q[CMWC_K];
    uint32_t c;
    uint32_t i;
};

enum {RNG_MT19937, RNG_TINYMT, RNG_XOR128, RNG_CMWC};

struct rng_state {
    int type;
    union {
        struct mt19937_state    mt19937;
        struct tinymt_state     tinymt;
        struct xor128_state     xor128;
        struct cmwc_state       cmwc;
    } state;
};

void rng_seed(struct rng_state *st, uint32_t s, int type);
/* [0, 2^32] */
uint32_t rng_u32(struct rng_state *st);
/* [0, 1) */
double rng_unit(struct rng_state *st);
/* [0, max) */
double rng_under(struct rng_state *st, int32_t max);
/* [min, max) */
double rng_between(struct rng_state *st, int32_t min, int32_t max);
/* min to max */
int32_t rng_range(struct rng_state *st, int32_t min, int32_t max);

enum {NOISE_SIMPLEX, NOISE_WAVELET, NOISE_PERLIN};

struct noise_state {
    int type;
    union {
        uint32_t p[512];
    } state;
    uint32_t octaves;
    double fallout;
};

void noise_seed(struct noise_state *ns, struct rng_state *st, int type);
void noise_detail(struct noise_state *ns, uint32_t octaves, double fallout);
double noise_generate(struct noise_state *ns, double x, double y, double z, double t);

#ifdef __cplusplus
}
#endif

#endif

