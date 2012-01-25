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
#include <SDL/SDL.h>
#include "at.h"

#define _ 0x000000
#define X 0xFFFFFF

#define BLANK           \
    {                   \
    _, _, _, _, _, _,   \
    _, _, _, _, _, _,   \
    _, _, _, _, _, _,   \
    _, _, _, _, _, _,   \
    _, _, _, _, _, _,   \
    _, _, _, _, _, _,   \
    }

static unsigned default_font[128][AT_FWIDTH * AT_FHEIGHT] = {
    /* 0 (null) - 31 (unit seperator) */
    BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK,
    BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK,
    BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK,
    BLANK, BLANK,
    /* 32 (blank) */
    {
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    /* 33 (!) - 47 (/) */
    {
    _, _, X, _, _, _,
    _, _, X, _, _, _,
    _, _, X, _, _, _,
    _, _, _, _, _, _,
    _, _, X, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, X, _, X, _, _,
    _, X, _, X, _, _,
    _, X, _, X, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, X, _, X, _, _,
    X, X, X, X, X, _,
    _, X, _, X, _, _,
    X, X, X, X, X, _,
    _, X, _, X, _, _,
    _, _, _, _, _, _,
    },
    {
    _, X, X, X, _, _,
    X, _, X, _, _, _,
    _, X, X, X, _, _,
    _, _, X, _, X, _,
    _, X, X, X, _, _,
    _, _, _, _, _, _,
    },
    {
    X, _, _, _, X, _,
    _, _, _, X, _, _,
    _, _, X, _, _, _,
    _, X, _, _, _, _,
    X, _, _, _, X, _,
    _, _, _, _, _, _,
    },
    {
    _, X, X, _, _, _,
    X, _, _, _, _, _,
    _, X, X, _, X, _,
    X, _, _, X, _, _,
    _, X, X, _, X, _,
    _, _, _, _, _, _,
    },
    {
    _, _, X, _, _, _,
    _, _, X, _, _, _,
    _, _, X, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, X, _, _, _,
    _, X, _, _, _, _,
    _, X, _, _, _, _,
    _, X, _, _, _, _,
    _, _, X, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, X, _, _, _,
    _, _, _, X, _, _,
    _, _, _, X, _, _,
    _, _, _, X, _, _,
    _, _, X, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, X, _, _, _,
    _, X, X, X, _, _,
    _, _, X, _, _, _,
    _, X, _, X, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, X, _, _, _,
    _, X, X, X, _, _,
    _, _, X, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, X, _, _, _,
    _, _, X, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, X, X, X, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, X, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, X, _, _,
    _, _, _, X, _, _,
    _, _, X, _, _, _,
    _, X, _, _, _, _,
    _, X, _, _, _, _,
    _, _, _, _, _, _,
    },
    /* 48 (0) - 57 (9) */
    {
    X, X, X, X, X, _,
    X, _, _, X, X, _,
    X, _, X, _, X, _,
    X, X, _, _, X, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    {
    _, _, X, _, _, _,
    _, X, X, _, _, _,
    _, _, X, _, _, _,
    _, _, X, _, _, _,
    _, X, X, X, _, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, _, _,
    _, _, _, _, X, _,
    _, X, X, X, _, _,
    X, _, _, _, _, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    _, _, _, _, X, _,
    _, X, X, X, _, _,
    _, _, _, _, X, _,
    X, X, X, X, _, _,
    _, _, _, _, _, _,
    },
    {
    X, _, _, _, _, _,
    X, _, _, _, _, _,
    X, _, X, _, _, _,
    X, X, X, X, _, _,
    _, _, X, _, _, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    X, _, _, _, _, _,
    X, X, X, X, _, _,
    _, _, _, _, X, _,
    X, X, X, X, _, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    X, _, _, _, _, _,
    X, X, X, X, X, _,
    X, _, _, _, X, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    _, _, _, _, X, _,
    _, _, _, X, _, _,
    _, _, X, _, _, _,
    _, _, X, _, _, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    X, _, _, _, X, _,
    X, X, X, X, X, _,
    X, _, _, _, X, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    X, _, _, _, X, _,
    X, X, X, X, X, _,
    _, _, _, _, X, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    /* 58 (:) - 64 (@) */
    {
    _, _, _, _, _, _,
    _, _, X, _, _, _,
    _, _, _, _, _, _,
    _, _, X, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, _, X, _, _, _,
    _, _, _, _, _, _,
    _, _, X, _, _, _,
    _, _, X, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, X, _, _,
    _, _, X, _, _, _,
    _, X, _, _, _, _,
    _, _, X, _, _, _,
    _, _, _, X, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, X, X, X, _, _,
    _, _, _, _, _, _,
    _, X, X, X, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, X, _, _, _, _,
    _, _, X, _, _, _,
    _, _, _, X, _, _,
    _, _, X, _, _, _,
    _, X, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, X, X, _, _, _,
    X, _, _, X, _, _,
    _, _, X, _, _, _,
    _, _, _, _, _, _,
    _, _, X, _, _, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    X, _, _, _, X, _,
    X, _, X, X, X, _,
    X, _, _, _, _, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    /* 65 (A) - 90 (Z) */
    {
    X, X, X, X, X, _,
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    X, X, X, X, X, _,
    X, _, _, _, X, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    X, _, _, _, X, _,
    X, X, X, X, _, _,
    X, _, _, _, X, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    X, _, _, _, _, _,
    X, _, _, _, _, _,
    X, _, _, _, _, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, _, _,
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    X, X, X, X, _, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    X, _, _, _, _, _,
    X, X, X, X, _, _,
    X, _, _, _, _, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    X, _, _, _, _, _,
    X, X, X, X, _, _,
    X, _, _, _, _, _,
    X, X, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    X, _, _, _, _, _,
    X, _, _, X, X, _,
    X, _, _, _, X, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    {
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    X, X, X, X, X, _,
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    _, _, X, _, _, _,
    _, _, X, _, _, _,
    _, _, X, _, _, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    {
    _, _, X, X, X, _,
    _, _, _, _, X, _,
    _, _, _, _, X, _,
    X, _, _, _, X, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    {
    X, _, _, _, X, _,
    X, _, _, X, _, _,
    X, X, X, _, _, _,
    X, _, _, X, _, _,
    X, _, _, _, X, _,
    _, _, _, _, _, _,
    },
    {
    X, _, _, _, _, _,
    X, _, _, _, _, _,
    X, _, _, _, _, _,
    X, _, _, _, _, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    {
    X, _, _, _, X, _,
    X, X, _, X, X, _,
    X, _, X, _, X, _,
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    _, _, _, _, _, _,
    },
    {
    X, _, _, _, X, _,
    X, X, _, _, X, _,
    X, _, X, _, X, _,
    X, _, _, X, X, _,
    X, _, _, _, X, _,
    _, _, _, _, _, _,
    },
    {
    _, X, X, X, _, _,
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    _, X, X, X, _, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, _, _,
    X, _, _, _, X, _,
    X, X, X, X, _, _,
    X, _, _, _, _, _,
    X, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    X, X, X, X, X, _,
    _, _, _, X, _, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, _, _,
    X, _, _, _, X, _,
    X, X, X, X, _, _,
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    X, _, _, _, _, _,
    X, X, X, X, X, _,
    _, _, _, _, X, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    _, _, X, _, _, _,
    _, _, X, _, _, _,
    _, _, X, _, _, _,
    _, _, X, _, _, _,
    _, _, _, _, _, _,
    },
    {
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    {
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    _, X, _, X, _, _,
    _, X, _, X, _, _,
    _, _, X, _, _, _,
    _, _, _, _, _, _,
    },
    {
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    X, _, X, _, X, _,
    X, _, X, _, X, _,
    _, X, _, X, X, _,
    _, _, _, _, _, _,
    },
    {
    X, _, _, _, X, _,
    _, X, _, X, _, _,
    _, _, X, _, _, _,
    _, X, _, X, _, _,
    X, _, _, _, X, _,
    _, _, _, _, _, _,
    },
    {
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    _, X, _, X, _, _,
    _, _, X, _, _, _,
    _, _, X, _, _, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    _, _, _, X, _, _,
    _, _, X, _, _, _,
    _, X, _, _, _, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    /* 91 ([) - 96 (`) */
    {
    _, X, X, _, _, _,
    _, X, _, _, _, _,
    _, X, _, _, _, _,
    _, X, _, _, _, _,
    _, X, X, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, X, _, _, _, _,
    _, X, _, _, _, _,
    _, _, X, _, _, _,
    _, _, _, X, _, _,
    _, _, _, X, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, X, X, _, _,
    _, _, _, X, _, _,
    _, _, _, X, _, _,
    _, _, _, X, _, _,
    _, _, X, X, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, X, _, _, _,
    _, X, _, X, _, _,
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
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    {
    _, X, _, _, _, _,
    _, _, X, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    /* 97 (a) - 122 (z) */
    {
    X, X, X, X, X, _,
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    X, X, X, X, X, _,
    X, _, _, _, X, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    X, _, _, _, X, _,
    X, X, X, X, _, _,
    X, _, _, _, X, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    X, _, _, _, _, _,
    X, _, _, _, _, _,
    X, _, _, _, _, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, _, _,
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    X, X, X, X, _, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    X, _, _, _, _, _,
    X, X, X, X, _, _,
    X, _, _, _, _, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    X, _, _, _, _, _,
    X, X, X, X, _, _,
    X, _, _, _, _, _,
    X, X, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    X, _, _, _, _, _,
    X, _, _, X, X, _,
    X, _, _, _, X, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    {
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    X, X, X, X, X, _,
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    _, _, X, _, _, _,
    _, _, X, _, _, _,
    _, _, X, _, _, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    {
    _, _, X, X, X, _,
    _, _, _, _, X, _,
    _, _, _, _, X, _,
    X, _, _, _, X, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    {
    X, _, _, _, X, _,
    X, _, _, X, _, _,
    X, X, X, _, _, _,
    X, _, _, X, _, _,
    X, _, _, _, X, _,
    _, _, _, _, _, _,
    },
    {
    X, _, _, _, _, _,
    X, _, _, _, _, _,
    X, _, _, _, _, _,
    X, _, _, _, _, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    {
    X, _, _, _, X, _,
    X, X, _, X, X, _,
    X, _, X, _, X, _,
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    _, _, _, _, _, _,
    },
    {
    X, _, _, _, X, _,
    X, X, _, _, X, _,
    X, _, X, _, X, _,
    X, _, _, X, X, _,
    X, _, _, _, X, _,
    _, _, _, _, _, _,
    },
    {
    _, X, X, X, _, _,
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    _, X, X, X, _, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, _, _,
    X, _, _, _, X, _,
    X, X, X, X, _, _,
    X, _, _, _, _, _,
    X, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    X, X, X, X, X, _,
    _, _, _, X, _, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, _, _,
    X, _, _, _, X, _,
    X, X, X, X, _, _,
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    X, _, _, _, _, _,
    X, X, X, X, X, _,
    _, _, _, _, X, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    _, _, X, _, _, _,
    _, _, X, _, _, _,
    _, _, X, _, _, _,
    _, _, X, _, _, _,
    _, _, _, _, _, _,
    },
    {
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    {
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    _, X, _, X, _, _,
    _, X, _, X, _, _,
    _, _, X, _, _, _,
    _, _, _, _, _, _,
    },
    {
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    X, _, X, _, X, _,
    X, _, X, _, X, _,
    _, X, _, X, X, _,
    _, _, _, _, _, _,
    },
    {
    X, _, _, _, X, _,
    _, X, _, X, _, _,
    _, _, X, _, _, _,
    _, X, _, X, _, _,
    X, _, _, _, X, _,
    _, _, _, _, _, _,
    },
    {
    X, _, _, _, X, _,
    X, _, _, _, X, _,
    _, X, _, X, _, _,
    _, _, X, _, _, _,
    _, _, X, _, _, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, _,
    _, _, _, X, _, _,
    _, _, X, _, _, _,
    _, X, _, _, _, _,
    X, X, X, X, X, _,
    _, _, _, _, _, _,
    },
    /* 123 ({) - 127 (DEL) */
    {
    _, _, X, X, _, _,
    _, _, X, _, _, _,
    _, X, _, _, _, _,
    _, _, X, _, _, _,
    _, _, X, X, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, X, _, _, _,
    _, _, X, _, _, _,
    _, _, X, _, _, _,
    _, _, X, _, _, _,
    _, _, X, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, X, X, _, _, _,
    _, _, X, _, _, _,
    _, _, _, X, _, _,
    _, _, X, _, _, _,
    _, X, X, _, _, _,
    _, _, _, _, _, _,
    },
    {
    _, _, _, _, _, _,
    _, X, _, _, _, _,
    X, _, X, _, X, _,
    _, _, _, X, _, _,
    _, _, _, _, _, _,
    _, _, _, _, _, _,
    },
    {
    X, X, X, X, X, X,
    X, X, X, X, X, X,
    X, X, X, X, X, X,
    X, X, X, X, X, X,
    X, X, X, X, X, X,
    X, X, X, X, X, X,
    },
};

static int keytab[SDLK_LAST] = {0};
static int pixel_width = 1;
static int pixel_height = 1;

static unsigned *font = (unsigned *)default_font;
static int font_width = AT_FWIDTH;
static int font_height = AT_FHEIGHT;

int at_open(int width, int height, char *title, int pw, int ph)
{
    pixel_width = pw;
    pixel_height = ph;

    SDL_Init(SDL_INIT_EVERYTHING);
    if (!SDL_SetVideoMode(width * pixel_width, height * pixel_height, 32, SDL_DOUBLEBUF))
        return -1;
    SDL_WM_SetCaption(title, NULL);
    SDL_EnableUNICODE(1);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    return 0;
}

void at_close(void)
{
    SDL_Quit();
}

int at_peek(int key)
{
    return keytab[key];
}

void at_plot(int x, int y, unsigned color)
{
    SDL_Rect rect;
    int r = (color >> 16) & 0xFF;
    int g = (color >> 8) & 0xFF;
    int b = (color >> 0) & 0xFF;
    color = SDL_MapRGB(SDL_GetVideoSurface()->format, r, g, b);

    rect.x = x * pixel_width;
    rect.y = y * pixel_height;
    rect.w = pixel_width;
    rect.h = pixel_height;
    SDL_FillRect(SDL_GetVideoSurface(), &rect, color);
}

void at_clear(unsigned color)
{
    int r, g, b;
    r = (color >> 16) & 0xFF;
    g = (color >> 8) & 0xFF;
    b = (color >> 0) & 0xFF;
    color = SDL_MapRGB(SDL_GetVideoSurface()->format, r, g, b);
    SDL_FillRect(SDL_GetVideoSurface(), NULL, color);
}

int at_flush(void)
{
    static SDL_Event ev;
    memset(keytab, 0, sizeof(keytab));
    SDL_Flip(SDL_GetVideoSurface());

    /* memset(keytab, 0, SDLK_LAST * sizeof(int)); */

    while (SDL_PollEvent(&ev)) {
        switch (ev.type) {
        case SDL_KEYDOWN:
            if (ev.key.keysym.unicode < SDLK_LAST)
                keytab[ev.key.keysym.unicode] = 1;
            keytab[ev.key.keysym.sym] = 1;
            break;
        case SDL_QUIT:
            return 0;
            break;
        default:
            break;
        }
    }
    return 1;
}

unsigned at_tick(unsigned by)
{
    if (!by)
        return SDL_GetTicks();
    SDL_Delay(by);
    return SDL_GetTicks();
}

void at_set_font(unsigned *f, int fw, int fh)
{
    if (f) {
        font = f;
        font_width = fw;
        font_height = fh;
    }
    else {
        font = (unsigned *)default_font;
        font_width = AT_FWIDTH;
        font_height = AT_FHEIGHT;
    }
}

void at_plot_glyph(int x, int y, int glyph, unsigned color)
{
    int fx, fy;
    unsigned g;

    for (fx = 0; fx < font_width; ++fx) {
        for (fy = 0; fy < font_height; ++fy) {
            g = font[glyph * font_width * font_height + fy * font_width + fx];
            if (g == 0xffffff)
                at_plot(x + fx, y + fy, color);
            else if (g != 0) {
                at_plot(x + fx, y + fy, g);
            }
        }
    }
}

void at_plot_glyphs(int x, int y, const char *glyphs, unsigned color)
{
    while (*glyphs) {
        at_plot_glyph(x, y, *glyphs, color);
        x += font_width;
        glyphs++;
    }
}