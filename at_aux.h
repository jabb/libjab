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
#ifndef AT_AUX_H
#define AT_AUX_H

#ifdef __cplusplus
extern "C" {
#endif

#define AT_AUX_FWIDTH       6
#define AT_AUX_FHEIGHT      6

enum {
    AT_AUX_SYM_HUMAN,
    AT_AUX_SYM_SPRUCE,
    AT_AUX_SYM_ELM,
    AT_AUX_SYM_BUSH,
    AT_AUX_SYM_SAPPLING,
    AT_AUX_SYM_PALM,
    AT_AUX_SYM_STOOL,
    AT_AUX_SYM_TABLE,
    AT_AUX_SYM_CHEST,
    AT_AUX_SYM_WALL,
    AT_AUX_SYM_FLOOR,
    AT_AUX_SYM_DOOR,
    AT_AUX_SYM_RABBIT,
    AT_AUX_SYM_COUNT = 32
};

extern unsigned at_aux_sym[AT_AUX_SYM_COUNT][AT_AUX_FWIDTH * AT_AUX_FHEIGHT];

#ifdef __cplusplus
}
#endif

#endif
