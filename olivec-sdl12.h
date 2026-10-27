// olivec-sdl12.h HOWTO
//
//	This file basically factors out 2 common functions used for SDL 1.2 with olive.c.
//	To look up all the definitions for the functions, search for "All functions" in
//	your text editor.
//
// -vmunix

#ifndef _OLIVE_SDL_1_2_
#define _OLIVE_SDL_1_2_
#include <SDL/SDL.h>
// All functions

// Draws a pixel to an SDL_Surface, usually the screen.
// Used for drawing from the olivec_canvas to the screen.
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

// This turns the olive.c hex colors (0xRRGGBBAA) into SDL 1.2's
// Uint32 color format. They seem like the same thing, but it's
// a little different.
Uint32 hex2uint32(SDL_Surface *surface, uint32_t hex);

#endif

#ifdef OLIVEC_SDL12_IMPLEMENTATION

/*
 * Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!
 */
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

Uint32 hex2uint32(SDL_Surface *surface, uint32_t hex) {
        Uint8 r = (hex >> 24) & 0xFF;
        Uint8 g = (hex >> 16) & 0xFF;
        Uint8 b = (hex >>  8) & 0xFF;
        Uint8 a =  hex        & 0xFF;

        return SDL_MapRGBA(surface->format, r, g, b, a);
}

#endif
