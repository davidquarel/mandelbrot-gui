#include "fractalgui.h"

int main(int argv, char **argc)
{
	SDL_Surface *display;
	SDL_Surface *image;
	SDL_Rect imagerect;
	SDL_Rect screenrect;
 
	/*initialize SDL video subsystem*/
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		/*error, quit*/
		exit(-1);
	}
 
	/*retrieve 640 pixel wide by 480 pixel high 8 bit display with video memory access*/
	display = SDL_SetVideoMode(480, 480, 8, SDL_HWSURFACE);
	/*check that surface was retrieved*/
	if(display == NULL){
		/*quit SDL*/
		SDL_Quit();
		exit(-1);
	}

	int padding = (4 - ((width * 3) % 4)) % 4;
	image = SDL_LoadBMP_RW(SDL_RWFromMem((void *)mandelbrot_bmp(480, 480), height * ((width * 3) + padding) + 26), 0);

	SDL_GetClipRect(image, &imagerect);
	SDL_GetClipRect(display, &screenrect);

	SDL_BlitSurface(image, &imagerect, display, &screenrect);
 
	/*inform screen to update its contents*/
	SDL_UpdateRect(display, 0, 0, 0, 0);
 
	/*wait 5 seconds (5000ms) before exiting*/
	SDL_Delay(5000);
 
	SDL_Quit();
 
	exit(0);
}

