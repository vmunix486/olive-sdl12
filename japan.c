// flag_jp.c
#define OLIVEC_IMPLEMENTATION
#include "olive.c"

#define OLIVEC_SDL12_IMPLEMENTATION
#include "olivec-sdl12.h"

#include <SDL/SDL.h>
#include <unistd.h>

#define WIDTH 900
#define HEIGHT 600

uint32_t pixels[WIDTH*HEIGHT];

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
