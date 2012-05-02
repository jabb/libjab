
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



#define N MT19937_K
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



static void mt_seed(mt19937_state *st, unsigned s)
{
    st->mt[0] = s & 0xffffffffU;
    for (st->mti = 1; st->mti < N; st->mti++)
        st->mt[st->mti] = (69069 * st->mt[st->mti - 1]) & 0xffffffffU;
}



static unsigned mt_random(mt19937_state *st)
{
    unsigned mag01[2] = {0x0, MATRIX_A};
    unsigned y;
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
        y = (st->mt[N - 1] & UPPER_MASK) | (st->mt[0]&LOWER_MASK);
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
Carry Multiply with Carry
\******************************************************************************/



static void cmwc_seed(cmwc_state *st, unsigned seed)
{
    unsigned i;

    srand(seed);
    for (i = 0; i < CMWC_K; ++i) {
        st->q[i] = rand();
    }
    st->c = (seed < 809430660) ? seed : 809430660 - 1;
    st->i = CMWC_K - 1;
}



static unsigned cmwc_random(cmwc_state *st)
{
    unsigned th, tl, q, qh, ql, a = 18782;
    unsigned x, r = 0xfffffffe;

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



void rng_seed(rng_state *st, unsigned s, unsigned type)
{
    st->type = type;

    switch (st->type) {
    case RNG_MT19937:
        mt_seed(&st->state.mt19937, s);
        break;

    case RNG_CMWC:
        /* Fallthrough. */
    default:
        cmwc_seed(&st->state.cmwc, s);
        break;
    }
}

unsigned rng_random_u(rng_state *st)
{
    switch (st->type) {
    case RNG_MT19937:
        return mt_random(&st->state.mt19937);

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

