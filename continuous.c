#define OLIVEC_IMPLEMENTATION
#include "olive.c"

#define OLIVEC_SDL12_IMPLEMENTATION
#include "olivec-sdl12.h"

#include <unistd.h>

#define WIDTH  640
#define HEIGHT 400
#define DEPTH  32

uint32_t pixels[WIDTH*HEIGHT];

struct {
	bool running;
	unsigned int scanx,scany;
	int rx,ry,dx,dy;
} info;

int main(void) {
	info.scanx=info.scany=10;
	info.running=true;
	info.dx=info.dy=1;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Could not initialize SDL.\n");
		return 1;
	}

	SDL_Surface *screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_SWSURFACE);

	Olivec_Canvas oc = olivec_canvas(pixels, WIDTH, HEIGHT, WIDTH);

	SDL_Event event;
	while (info.running) {
		SDL_LockSurface(screen);

		olivec_fill(oc, 0xFF131313);

		olivec_rect(oc, info.rx, info.ry, 10, 10, 0xFF0000FF);

		info.rx+=info.dx;info.ry+=info.dy;

		if (info.rx >= WIDTH - 10) info.dx=-info.dx;
		else if (info.ry <= 0) info.dy=abs(info.dy);
		else if (info.rx <= 0) info.dx=abs(info.dx);
		else if (info.ry >= HEIGHT - 10) info.dy=-info.dy;

		for (long scan = 0; scan != WIDTH * HEIGHT; scan++) {
			putpixel(screen, info.scanx, info.scany,
				hex2uint32(screen, pixels[scan]));

			info.scanx++;

			if (info.scanx == WIDTH) {
				info.scanx = 0;
				info.scany++;
			}
		} info.scanx=info.scany=0;

		SDL_UpdateRect(screen,0,0,0,0);
		SDL_UnlockSurface(screen);

		while (SDL_PollEvent(&event)) {
                        switch (event.type) {
                                case SDL_QUIT:
                                        info.running = false; break;
                                case SDL_KEYDOWN:
                                        switch(event.key.keysym.sym) {
                                                case SDLK_ESCAPE:
                                                        info.running = false;
                                                        break;
                                                default: break;
                                        }
                                default: break;
                        }
                }


//		usleep(60000);
	}
}
