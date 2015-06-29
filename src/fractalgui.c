#include "fractalgui.h"
#define TICK_INTERVAL 20
#define ITER_MAX_INCREASE 1.05
/* (50 fps) */

int main(int argc, char *argv[])
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
	struct mandelparams p;
	char *im;

	p.center_x = 0;
	p.center_y = 0;
	p.scale_factor_x = 2 * 2 / (double)1024 + 1;
	p.scale_factor_y = 2 * 2 / (double)1024 + 1;
	p.radius = 2;
	p.bound = 4;
	p.iter_max = 100;
	p.color_step = 5;

	white.r = 255;
	white.g = 255;
	white.b = 255;

	if (argc == 1) {
		p.width = 640;
		p.height = 640;
	} else if (argc == 3) {
		if (ispnum(argv[1]) && ispnum(argv[2])) {
			p.width = atoi(argv[1]);
			p.height = atoi(argv[2]);
		}
	} else {
		print_usage_exit();
	}
 
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		perror("Error initialising SDL");
		exit(-1);
	}
 
	display = SDL_SetVideoMode(p.width, p.height, 8, SDL_HWSURFACE);
	if(display == NULL){
		perror("Error initialising display");
		SDL_Quit();
		exit(-1);
	}

	SDL_WM_SetCaption("Mandelbrot", 0);

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

	padding = (4 - ((p.width * 3) % 4)) % 4;
	im = mandelbrot_bmp(p);
	image = SDL_LoadBMP_RW(SDL_RWFromMem((void *)im, p.height * ((p.width * 3) + padding) + 26), 0);

	SDL_GetClipRect(image, &imagerect);
	SDL_GetClipRect(display, &screenrect);

	SDL_BlitSurface(image, &imagerect, display, &screenrect);
 
	SDL_UpdateRect(display, 0, 0, 0, 0);
 
	loop = 1;
	while (loop) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				loop = 0;
				break;
			} else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
					case SDLK_q:
						loop = 0;
						break;
					default:
						break;
				}
			} else if (event.type == SDL_MOUSEBUTTONDOWN) {
				SDL_FreeSurface(image);
				free(im);
				/* move viewpoint here */
				p.center_x = p.center_x + ((((double)event.button.x / ((double)p.width / 2.0)) - 1) * p.radius);
				p.center_y = p.center_y + ((((double)event.button.y / ((double)p.width / 2.0)) - 1) * p.radius);
				if (event.button.button == SDL_BUTTON_LEFT) {
					p.radius /= 2;
					p.iter_max *= ITER_MAX_INCREASE;
				} else if (event.button.button == SDL_BUTTON_RIGHT) {
					p.radius *= 2;
					p.iter_max /= ITER_MAX_INCREASE;
				}
				im = mandelbrot_bmp(p);
				image = SDL_LoadBMP_RW(SDL_RWFromMem((void *)im, p.height * ((p.width * 3) + padding) + 26), 0);
				SDL_BlitSurface(image, &imagerect, display, &screenrect);

			}
		}
		SDL_UpdateRect(display, 0, 0, 0, 0);
		SDL_Delay(TICK_INTERVAL);
	}

	SDL_FreeSurface(display);
 
	SDL_Quit();
 
	exit(0);
}

void print_usage_exit()
{
	printf("Usage:\nfractalgui [<width> <height>]\n");
	exit(1);
}

int ispnum(char *s)
{
	int len = strlen(s);
	for (int i = 0; i < len; i++) {
		if (!isdigit(s[i])) {
			return 0;
		}
	}
	return 1;
}

