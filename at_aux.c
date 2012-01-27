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
#include "at_aux.h"

#define _ 0x000000
#define X 0xFFFFFF
#define N 0x964B00

unsigned at_aux_sym[AT_AUX_SYM_COUNT][AT_AUX_FWIDTH * AT_AUX_FHEIGHT] = {
    {
    _, _, _, _, _, _,
    _, _, X, _, _, _,
    _, X, X, X, _, _,
    _, _, X, _, _, _,
    _, X, _, X, _, _,
    _, _, _, _, _, _,
    },
    {
    X, _, _, _, _, _,
    X, _, X, _, _, _,
    X, X, X, X, _, _,
    _, _, X, _, _, _,
    _, X, _, X, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, X, _, _, _,
    _, X, X, X, _, _,
    _, _, N, _, _, _,
    _, X, X, X, _, _,
    X, _, N, _, X, _,
    _, _, _, _, _, _,
    },
    {
    _, _, X, _, _, _,
    _, X, X, X, _, _,
    X, X, X, X, X, _,
    _, _, N, _, _, _,
    _, _, N, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, X, _, _, _,
    _, X, X, X, _, _,
    _, X, X, X, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, X, _, _, _,
    _, X, X, X, _, _,
    X, X, X, X, X, _,
    _, _, N, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, X, X, X, _, _,
    X, _, N, _, X, _,
    _, _, N, _, _, _,
    _, _, N, _, _, _,
    _, _, N, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, X, X, X, _, _,
    _, X, _, X, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    X, X, X, X, X, _,
    _, X, _, X, _, _,
    _, X, _, X, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, X, X, X, _, _,
    X, X, X, X, X, _,
    X, _, _, _, X, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    {
    _, X, _, X, _, _,
    X, X, X, X, X, _,
    _, X, X, X, _, _,
    X, X, X, X, X, _,
    _, X, _, X, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, X, X, X, _, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    X, _, _, X, X, _,
    X, X, X, X, X, _,
    X, _, _, X, X, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, X, _, _, _,
    _, X, _, _, _, _,
    _, X, X, X, _, _,
    _, _, X, X, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
};
