// flag_jp.c
#define OLIVEC_IMPLEMENTATION
#include "olive.c"

#include <SDL/SDL.h>
#include <unistd.h>

#define WIDTH 900
#define HEIGHT 600

uint32_t pixels[WIDTH*HEIGHT];

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

int main(void) {
	if (SDL_Init(SDL_INIT_VIDEO) == -1) {
		fprintf(stderr, "Could not initialize SDL 1.2.\n");
		return 1;
	}

	SDL_Surface *screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE);

	Olivec_Canvas oc = olivec_canvas(pixels, WIDTH, HEIGHT, WIDTH);
	// Taken from https://upload.wikimedia.org/wikipedia/en/9/9e/Flag_of_Japan.svg
	olivec_fill(oc, 0xFFFFFFFF);
	olivec_circle(oc, WIDTH/2, HEIGHT/2, 180, 0xFF2D00BC);

	if (SDL_MUSTLOCK(screen)) {
		if (SDL_LockSurface(screen) < 0) {
			fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
			return 1;
		}
	}

	int scanx = 0;
	int scany = 0;
	for (long scan = 0; scan != WIDTH * HEIGHT; scan++) {
		putpixel(screen, scanx, scany, 
			hex2uint32(screen, pixels[scan]));
		
		scanx++;

		if (scanx == WIDTH) {
			scanx = 0;
			scany++;
		}
	}

	if (SDL_MUSTLOCK(screen))
		SDL_UnlockSurface(screen);

	SDL_UpdateRect(screen, 0, 0, 0, 0);

	SDL_Event event;

	bool running = true;
	while (running) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					running = false; break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_ESCAPE:
							running = false;
							break;
						default: break;
					}
				default: break;
			}
		}
	}

	return 0;
}
