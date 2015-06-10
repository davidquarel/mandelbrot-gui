/* Mandelbrot Set
 * David Quarel 07/06/15
 * Pipe output from code into a .ppm file
 * Got zoom working
 * Color is now parallelized
 */

/* BENCHMARK
 * time ./mandelbrot -d 8192 8192 -f c -i 150 -z -0.16 1.033 0.015 > test.ppm
 * this took partch 3m16s
 */

/* Zoom in on the coordinate point
 * 0.001643721971153 -0.822467633298876i
 * for pretty tri squiggles
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>
#include <omp.h>
#ifndef DCOMPLEX_H
#include "dcomplex.h"
#define DCOMPLEX_H
#endif

#define input_eq(s1) !(strcmp(argv[i],s1))

const double BOUND = 4; /* square roots are expensive */
int ITER_MAX = 100;
int COLOR_STEP = 11;

int WIDTH = 1024;
int HEIGHT = 1024;
/* By Default, centers in on (0,0), radius 1 */
double CENTER_X = 0;
double CENTER_Y = 0;
double RADIUS = 1;
/* Default values for julia set iteration */
double JULIA_X = 0.3515;
double JULIA_Y = 0.42193;

double SCALE_FACTOR_X = 2 * 2 / (double)1024 + 1;
double SCALE_FACTOR_Y = 2 * 2 / (double)1024 + 1;

typedef struct {
	char r;
	char g;
	char b;
} color;

color make_color(int red, int green, int blue)
{
	color new_color = {red, green, blue};
	return new_color;
}

/* 0 if in set, 1 else, B&W */
void mandelbrot_bw()
{
	char *image = (char *)malloc(WIDTH * HEIGHT * sizeof(char) * 2 + HEIGHT + 16);
	char *start = image + sprintf(image, "P1\n%d %d\n", WIDTH, HEIGHT);
	char *current;

	int x;
	int y;
	complex c;
	#pragma omp parallel private(x, y, c, current) shared(image)
	{
		#pragma omp for
		for (y = 0; y < HEIGHT; y++) {
			current = start + (y * HEIGHT * 2 + y);
			for (x = 0; x < WIDTH; x++) {
				c = coord_to_complex(x, y, WIDTH, HEIGHT, CENTER_X, CENTER_Y, RADIUS);
				int iter;

				complex z = make_complex(0, 0);

				for (iter = ITER_MAX; iter > 0; iter--) {
					if (mag_sq(z) >= BOUND) {
						/* point escaped BOUNDs */
						*(current++) = '0';
						break;
					}
					z = add(mult(z, z), c);  /* iterate again, z_{i+1} = z_i^2 + c */
				}
				if (!iter) {
					/* printf("1 "); //point stayed in set after ITER_LIMIT_BW iterations, in set */
					*(current++) = '1';
				}
				*(current++) = ' ';
			}
			*current = '\n';
		}
	}
	puts(image);
	free(image);
	return;
}

/* 8-bit binary greyscale binary */
void mandelbrot_gs()
{
	char *image = (char *)malloc(WIDTH * HEIGHT * sizeof(char) + 50);
	int pre = sprintf(image, "P5\n%d %d\n255\n", WIDTH, HEIGHT);

	char *start = image + pre;
	char *current;

	int x;
	int y;
	complex c;
	#pragma omp parallel private(x, y, c, current)
	{ 
		#pragma omp for
		for (y = 0; y < HEIGHT; y++) {
			current = start + (y * HEIGHT);
			for (x = 0; x < WIDTH; x++) {
				c = coord_to_complex(x, y, WIDTH, HEIGHT, CENTER_X, CENTER_Y, RADIUS);
				int iter;

				complex z = make_complex(0, 0);

				for (iter = ITER_MAX; iter > 0; iter--) {
					if (mag_sq(z) >= BOUND) {
						/* putchar(iter); */
						*(current++) = iter;
						break;
					}
					z = add(mult(z, z), c);  /* z_{i+1} = z_i^2 + c */
				}
				if (!iter) {
					/* putchar(0); */
					*(current++) = 0;
				}
			}
		}
	}
	write(1, image, pre + HEIGHT * WIDTH);
	free(image);
	return;
}

/* 24-bit, colour changes with iteration depth, binary */
void mandelbrot_col()
{
	char *image = (char *)malloc(HEIGHT * WIDTH * sizeof(color) + 50);
	int pre = sprintf(image, "P6\n%d %d\n255\n", WIDTH, HEIGHT); /* P6, colour binary file */
	color *world = (color *)(image + pre);
	color *current = world;
	int x;
	int y;
	complex c;
	complex z;
	#pragma omp parallel private(x, y, c, z, current)
	{
		for (y = 0; y < HEIGHT; y++) {
			#pragma omp for
			for (x = 0; x < WIDTH; x++) {
				current = world + (y * WIDTH) + x;
				c = coord_to_complex(x, y, WIDTH, HEIGHT, CENTER_X, CENTER_Y, RADIUS);
				z = make_complex(0, 0);
				int iter;
				for (iter = ITER_MAX; iter > 0; iter--) {
					if (mag_sq(z) >= BOUND) {
						char val = iter * COLOR_STEP;
						*current = make_color(val*2, val, val);
						break;
					}
					z = add(mult(z, z), c);  /* z_{i+1} = z_i^2 + c */
				}
				if (!iter) {
					*current = make_color(0, 0, 0);
				}
			}
		}
	}
	write(1, image, HEIGHT * WIDTH * sizeof(color) + pre);
	free(image);

	return;
}

/* 24-bit, colour changes with iteration depth, binary */
void julia_col()
{
	complex JULIA_C = make_complex(JULIA_X, JULIA_Y);
	char *image = (char *)malloc(HEIGHT * WIDTH * sizeof(color) + 50);
	int pre = sprintf(image, "P6\n%d %d\n255\n", WIDTH, HEIGHT); /* P6, colour binary file */
	color *world = (color *)(image + pre);
	color *current = world;
	int x;
	int y;
	complex z;
	#pragma omp parallel private(x, y, z, current)
	{
		for (y = 0; y < HEIGHT; y++) {
			#pragma omp for
			for (x = 0; x < WIDTH; x++) {
				current = world + (y * WIDTH) + x;
				z = coord_to_complex(x, y, WIDTH, HEIGHT, CENTER_X, CENTER_Y, RADIUS);
				int iter;
				for (iter = ITER_MAX; iter > 0; iter--) {
					if (mag_sq(z) >= BOUND) {
						char val = iter * COLOR_STEP;
						*current = make_color(val, val, val*2);
						break;
					}
					z = add(mult(z, z), JULIA_C);  /* z_{i+1} = z_i^2 + c */
				}
				if (!iter) {
					*current = make_color(0, 0, 0);
				}
			}
		}
	}

	write(1, image, HEIGHT * WIDTH * sizeof(color) + pre);
	free(image);

	return;

}

void print_help()
{
	printf("usage: mandelbrot [-h | -help] [-window WIDTH HEIGHT]" 
			"[-mandel] [-julia X Y] [-bw | -grey | -color | -color-step STEP]"
			"[-iter I] [-origin X Y] [-radius R] > file.ppm\n"
			"-h | --help: Print this help\n"
			"-window WIDTH HEIGHT: dimensions of image, default 1024x1024\n"
			"-mandel: Draw the mandelbrot set\n"
			"-julia X Y: Draw the Julia set with initial value C = X + Yi, default C = -0.778 - 0.116i \n"
			"-bw: 1-bit B&W, ASCII output\n"
			"-grey: 8-bit Greyscale, Binary output\n"
			"-color: 24-bit colour, Binary output\n"
			"-color-step STEP: 24-bit colour, Binary output, manual colour step, default 11\n"
			"-iter I: Iterations for mandelbrot recursion, default 100\n"
			"-origin X Y: Location to center output image about\n"
			"-radius R: Output image will be the bounding box [X-R,Y-R] to [X+R,Y+R]\n"
			"Will default to drawing bounding box [-1,-1] to [1,1]\n"
			"Pipe the result into file.ppm\n"
			);
	return;
}

void print_usage()
{
	printf("usage: mandelbrot [-h | -help] [-window WIDTH HEIGHT] " 
			"[-mandel] [-julia X Y] [-bw | -grey | -color | -color-step STEP] "
			"[-iter I] [-origin X Y] [-radius R] > file.ppm\n");
	return;
}

int main(int argc, char *argv[])
{
	char format = 'g'; /* Default greyscale */
	char algorithm = 'm'; /* Default mandelbrot */
	int i;
	if (argc == 1) {
		print_usage();
		return 0;
	}
	for (i = 1; i < argc; i++) {
		/* print halp */
		if (input_eq("-h") || input_eq("--help")) {
			print_help();
			return 0;
		}
		/* read window size */
		else if (input_eq("-window")) { 
			sscanf(argv[i + 1], "%d", &WIDTH);
			sscanf(argv[i + 2], "%d", &HEIGHT);
			i += 2;
		}
		/* select algorithm */
		else if (input_eq("-mandel")) {
			algorithm = 'm';
		}
		/* read value of c = X + Yi for the julia set */
		else if (input_eq("-julia")) {
			algorithm = 'j'; 
			sscanf(argv[i + 1], "%lf", &JULIA_X);
			sscanf(argv[i + 2], "%lf", &JULIA_Y);
			i += 2;
		}
		/* read format */
		else if (input_eq("-bw")) {
			format = 'b';
		}
		else if (input_eq("-grey")) {
			format = 'g';
		}
		else if (input_eq("-color")) {
			format = 'c';
		}
		/* read color step */
		else if (input_eq("-color-step")) {
			format = 'c';
			sscanf(argv[i + 1], "%d", &COLOR_STEP);
			i++;
		}	
		/* read iter */
		else if (input_eq("-iter")) { 
			sscanf(argv[i + 1], "%d", &ITER_MAX);
			i++;
		}
		/* read origin */
		else if (input_eq("-origin")) { 
			sscanf(argv[i + 1], "%lf", &CENTER_X);
			sscanf(argv[i + 2], "%lf", &CENTER_Y);
			i += 2;
		}
		/* read radius */
		else if (input_eq("-radius")) {
			sscanf(argv[i + 1], "%lf", &RADIUS);
			i++;
		} else {
			/* error, did not match */
			printf("Cannot parse args.\n");
			return 1;
		}
	}

	SCALE_FACTOR_X = 2 * RADIUS / (double)WIDTH + 1;
	SCALE_FACTOR_Y = 2 * RADIUS / (double)HEIGHT + 1;
	
	if (algorithm == 'j') {
		julia_col();
	} else if (format == 'b') { /* 1-bit black and white mandelbrot */
		mandelbrot_bw();
	}
	else if (format == 'g') { /* 8-bit greyscale mandelbrot */
		mandelbrot_gs();	
	}
	else if (format == 'c') { /* 24-bit colour, changes colour with iteration depth */
		mandelbrot_col();
	}

	return 0;
}

