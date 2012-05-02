
#include "random.h"

#include <stdlib.h>

/******************************************************************************\
Mersenne Twister
\******************************************************************************/



/* A C-program for MT19937: Real number version                */
/*   genrand() generates one pseudorandom real number (double) */
/* which is uniformly distributed on [0,1]-interval, for each  */
/* call. sgenrand(seed) set initial values to the working area */
/* of 624 words. Before genrand(), sgenrand(seed) must be      */
/* called once. (seed is any 32-bit integer except for 0).     */
/* Integer generator is obtained by modifying two lines.       */
/*   Coded by Takuji Nishimura, considering the suggestions by */
/* Topher Cooper and Marc Rieffel in July-Aug. 1997.           */

/* This library is free software; you can redistribute it and/or   */
/* modify it under the terms of the GNU Library General Public     */
/* License as published by the Free Software Foundation; either    */
/* version 2 of the License, or (at your option) any later         */
/* version.                                                        */
/* This library is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of  */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.            */
/* See the GNU Library General Public License for more details.    */
/* You should have received a copy of the GNU Library General      */
/* Public License along with this library; if not, write to the    */
/* Free Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA   */
/* 02111-1307  USA                                                 */

/* Copyright (C) 1997 Makoto Matsumoto and Takuji Nishimura.       */
/* Any feedback is very welcome. For any question, comments,       */
/* see http://www.math.keio.ac.jp/matumoto/emt.html or email       */
/* matumoto@math.keio.ac.jp                                        */



#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfU
#define UPPER_MASK 0x80000000U
#define LOWER_MASK 0x7fffffffU

#define TEMPERING_MASK_B 0x9d2c5680U
#define TEMPERING_MASK_C 0xefc60000U
#define TEMPERING_SHIFT_U(y)  (y >> 11)
#define TEMPERING_SHIFT_S(y)  (y << 7)
#define TEMPERING_SHIFT_T(y)  (y << 15)
#define TEMPERING_SHIFT_L(y)  (y >> 18)



static void mt_seed(mt19937_state *st, uint32_t seed)
{
    st->mt[0] = seed & 0xffffffffU;
    for (st->mti = 1; st->mti < N; st->mti++)
        st->mt[st->mti] = (69069 * st->mt[st->mti - 1]) & 0xffffffffU;
}



static uint32_t mt_random(mt19937_state *st)
{
    static uint32_t mag01[2] = {0x0, MATRIX_A};
    uint32_t y;
    int kk;

    if (st->mti >= N) {
        for (kk = 0; kk < N - M; kk++) {
            y = (st->mt[kk] & UPPER_MASK) | (st->mt[kk + 1] & LOWER_MASK);
            st->mt[kk] = st->mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1];
        }
        for (; kk < N - 1; kk++) {
            y = (st->mt[kk] & UPPER_MASK) | (st->mt[kk + 1] & LOWER_MASK);
            st->mt[kk] = st->mt[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1];
        }
        y = (st->mt[N - 1] & UPPER_MASK) | (st->mt[0] & LOWER_MASK);
        st->mt[N - 1] = st->mt[M - 1] ^ (y >> 1) ^ mag01[y & 0x1];

        st->mti = 0;
    }

    y = st->mt[st->mti++];
    y ^= TEMPERING_SHIFT_U(y);
    y ^= TEMPERING_SHIFT_S(y) & TEMPERING_MASK_B;
    y ^= TEMPERING_SHIFT_T(y) & TEMPERING_MASK_C;
    y ^= TEMPERING_SHIFT_L(y);

    return y;
}

/******************************************************************************\
Tiny Mersenne Twister
\******************************************************************************/



/*
Copyright (c) 2011 Mutsuo Saito, Makoto Matsumoto, Hiroshima
University and The University of Tokyo. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of the Hiroshima University nor the names of
      its contributors may be used to endorse or promote products
      derived from this software without specific prior written
      permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/



#define TINYMT32_MEXP 127
#define TINYMT32_SH0 1
#define TINYMT32_SH1 10
#define TINYMT32_SH8 8
#define TINYMT32_MASK UINT32_C(0x7fffffff)
#define TINYMT32_MUL (1.0f / 4294967296.0f)



#define MIN_LOOP 8
#define PRE_LOOP 8



static void period_certification(tinymt_state *st) {
    if ((st->status[0] & TINYMT32_MASK) == 0 &&
        st->status[1] == 0 &&
        st->status[2] == 0 &&
        st->status[3] == 0) {

        st->status[0] = 'T';
        st->status[1] = 'I';
        st->status[2] = 'N';
        st->status[3] = 'Y';
    }
}



static void tinymt_next_state(tinymt_state *st)
{
    uint32_t x;
    uint32_t y;

    y = st->status[3];
    x = (st->status[0] & TINYMT32_MASK)
    ^ st->status[1]
    ^ st->status[2];
    x ^= (x << TINYMT32_SH0);
    y ^= (y >> TINYMT32_SH0) ^ x;
    st->status[0] = st->status[1];
    st->status[1] = st->status[2];
    st->status[2] = x ^ (y << TINYMT32_SH1);
    st->status[3] = y;
    st->status[1] ^= -((int32_t)(y & 1)) & st->mat1;
    st->status[2] ^= -((int32_t)(y & 1)) & st->mat2;
}



static void tinymt_seed(tinymt_state *st, uint32_t seed)
{
    int i;
    st->status[0] = seed;
    st->status[1] = st->mat1;
    st->status[2] = st->mat2;
    st->status[3] = st->tmat;
    for (i = 1; i < MIN_LOOP; i++) {
        st->status[i & 3] ^= i + UINT32_C(1812433253) *
                             (st->status[(i - 1) & 3] ^
                             (st->status[(i - 1) & 3] >> 30));
    }
    period_certification(st);
    for (i = 0; i < PRE_LOOP; i++) {
        tinymt_next_state(st);
    }
}



static uint32_t tinymt_temper(tinymt_state *st) {
    uint32_t t0, t1;
    t0 = st->status[3];
#if defined(LINEARITY_CHECK)
    t1 = st->status[0]
    ^ (st->status[2] >> TINYMT32_SH8);
#else
    t1 = st->status[0]
    + (st->status[2] >> TINYMT32_SH8);
#endif
    t0 ^= t1;
    t0 ^= -((int32_t)(t1 & 1)) & st->tmat;
    return t0;
}



static uint32_t tinymt_random(tinymt_state *st) {
    tinymt_next_state(st);
    return tinymt_temper(st);
}

/******************************************************************************\
Xor128
\******************************************************************************/



static void xor128_seed(xor128_state *st, uint32_t seed)
{
    int i;

    srand(seed);
    for (i = 0; i < XOR128_K; ++i) {
        st->q[i] = rand();
    }
}



static uint32_t xor128_random(xor128_state *st)
{
    uint32_t t;
    t = (st->q[0] ^ (st->q[0] << 11));
    st->q[0] = st->q[1];
    st->q[1] = st->q[2];
    st->q[2] = st->q[3];
    return st->q[3] = st->q[3] ^ (st->q[3] >> 19) ^ (t ^ (t >> 8));
}

/******************************************************************************\
Complementary Multiply with Carry
\******************************************************************************/



static void cmwc_seed(cmwc_state *st, uint32_t seed)
{
    int i;

    srand(seed);
    for (i = 0; i < CMWC_K; ++i) {
        st->q[i] = rand();
    }
    st->c = (seed < 809430660) ? seed : 809430660 - 1;
    st->i = CMWC_K - 1;
}



static uint32_t cmwc_random(cmwc_state *st)
{
    uint32_t th, tl, q, qh, ql, a = 18782;
    uint32_t x, r = 0xfffffffe;

    st->i = (st->i + 1) & (CMWC_K - 1);

    q = st->q[st->i];
    qh = q >> 16;
    ql = q & 0xffff;

    tl = a * q + st->c;
    th = (a * qh + (((st->c & 0xffff) + a * ql) >> 16) + (st->c >> 16)) >> 16;


    st->c = th;
    x = tl + th;
    if (x < st->c) {
        x++;
        st->c++;
    }
    return st->q[st->i] = r - x;
}



/******************************************************************************\
Generic RNG
\******************************************************************************/



void rng_seed(rng_state *st, uint32_t seed, int type)
{
    st->type = type;

    switch (st->type) {
    case RNG_MT19937:
        mt_seed(&st->state.mt19937, seed);
        break;

    case RNG_TINYMT:
        tinymt_seed(&st->state.tinymt, seed);
        break;

    case RNG_XOR128:
        xor128_seed(&st->state.xor128, seed);
        break;

    case RNG_CMWC:
        /* Fallthrough. */
    default:
        cmwc_seed(&st->state.cmwc, seed);
        break;
    }
}



uint32_t rng_random_u(rng_state *st)
{
    switch (st->type) {
    case RNG_MT19937:
        return mt_random(&st->state.mt19937);

    case RNG_TINYMT:
        return tinymt_random(&st->state.tinymt);

    case RNG_XOR128:
        return xor128_random(&st->state.xor128);

    case RNG_CMWC:
        return cmwc_random(&st->state.cmwc);

    default:
        return 0;
    }

}



double rng_random_d(rng_state *st)
{
    return rng_random_u(st) * 2.3283064365386963e-10;
}
