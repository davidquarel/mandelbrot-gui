#include "fractalgui.h"
#define TICK_INTERVAL 20
/* (50 fps) */

int main(int argv, char **argc)
{
	SDL_Surface *display;
	SDL_Surface *image;
	SDL_Surface *loading;
	SDL_Rect imagerect;
	SDL_Rect screenrect;
	SDL_Event event;
	SDL_Color white;
	TTF_Font *font;
	int padding;
	int loop;

	white.r = 255;
	white.g = 255;
	white.b = 255;
 
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		perror("Error initialising SDL");
		exit(-1);
	}
 
	display = SDL_SetVideoMode(480, 480, 8, SDL_HWSURFACE);
	if(display == NULL){
		perror("Error initialising display");
		SDL_Quit();
		exit(-1);
	}

	if (TTF_Init() < 0) {
		perror("Error initialising SDL_ttf");
		SDL_FreeSurface(display);
		SDL_Quit();
		exit(-1);
	}

	font = TTF_OpenFont("../fonts/LiberationMono-Regular.ttf", 28);
	if (font == 0) {
		perror("Error finding LiberationMono-Regular.ttf font");
		TTF_Quit();
		SDL_FreeSurface(display);
		SDL_Quit();
		exit(-1);
	}

	loading = TTF_RenderText_Solid(font, "Loading...", white);
	SDL_BlitSurface(loading, 0, display, 0);
	SDL_UpdateRect(display, 0, 0, 0, 0);

	padding = (4 - ((width * 3) % 4)) % 4;
	image = SDL_LoadBMP_RW(SDL_RWFromMem((void *)mandelbrot_bmp(480, 480), height * ((width * 3) + padding) + 26), 0);

	SDL_GetClipRect(image, &imagerect);
	SDL_GetClipRect(display, &screenrect);

	SDL_BlitSurface(image, &imagerect, display, &screenrect);
 
	SDL_UpdateRect(display, 0, 0, 0, 0);
 
	loop = 1;
	while (loop) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
					case SDLK_q:
						loop = 0;
						break;
					default:
						break;
				}
			}
		}
		SDL_UpdateRect(display, 0, 0, 0, 0);
		SDL_Delay(TICK_INTERVAL);
	}
 
	SDL_Quit();
 
	exit(0);
}

