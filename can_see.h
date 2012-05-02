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
#ifndef CAN_SEE_H
#define CAN_SEE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  path_to
 * Bresenham's line argorithm used to build a path.
 *
 * x0   Origin's X coordinate.
 * y0   Origin's Y coordinate.
 * x1   Destination's X coordinate.
 * y1   Destination's Y coordinate.
 * path A 1D array of arrays of length 2. (int[][2]). (-1, -1) is end of path.
 * sz   The size of the path.
 */
void path_to(int x0, int y0, int x1, int y1, int *path, int sz);

/**
 *  can_see
 * Slow but simple line casting algorithm for vision on a 2D grid.
 *
 * x0   Origin's X coordinate.
 * y0   Origin's Y coordinate.
 * x1   Spotting X coordinate.
 * y1   Spotting Y coordinate.
 * vis  How far can we see? 0-255
 * sight    Treated as a 2D. 0's are not obstructing locations, 1's are.
 * width    Width of the 2D array. Index = Y*Width+X.
 *
 * Returns 1 if can be seen, 0 if can't.
 */
int can_see(int x0, int y0, int x1, int y1, int vis, int *sight, int width);

#ifdef __cplusplus
}
#endif

#endif

