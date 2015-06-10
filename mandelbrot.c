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

#define input_eq(s1) !(strcmp(argv[i],s1))

const double BOUND = 4; //square roots are expensive
int ITER_MAX = 255;
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
	double re;
	double im;
} complex;

/* Functions for Complex Numbers */
complex make_complex(double real, double imag)
{
	complex new_complex = {real, imag};
	return new_complex;
}

complex coord_to_complex(int x, int y)
{
	double new_x;
	double new_y;
	new_x = (double)x * 2 * RADIUS / (double)WIDTH + CENTER_X - RADIUS; //scale [0,SIZE] to [X-R,X+R]
	new_y = (double)y * 2 * RADIUS / (double)HEIGHT - CENTER_Y - RADIUS; //scale [0,SIZE] to [Y-R,Y+R]
	return make_complex(new_x, new_y);
}

complex add(complex a, complex b)
{
	return make_complex(a.re + b.re, a.im + b.im);
}

complex mult(complex a, complex b)
{
	return make_complex(a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re);
}

double mag_sq(complex a)
{
	return (a.re * a.re + a.im * a.im);
}

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
				c = coord_to_complex(x, y);
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
				c = coord_to_complex(x, y);
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
				c = coord_to_complex(x, y);
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
				z = coord_to_complex(x, y);
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
	printf("usage: mandelbrot [-h | --help] [-d W H] [-a m|j] [-j X Y] [-f b|g|c] [-s S] [-i I] [-p X Y] [-r R]\n"
			"-h | --help: Print this help\n"
			"-d: dimensions of image, default 1024x1024\n"
			"W/H: WIDTH/HEIGHT of image\n"
			"\n"
			"-a: Choose algorithm to draw picture\n"
			"m: Mandelbrot Set\n"
			"j: Julia Set and initial value for c\n"
			"\n"
			"-j X Y Choose value of c = X+Yi for computing the Julia set"
			"\n"
			"-f: choose format, default greyscale\n"
			"b: 1-bit B&W, ASCII output\n"
			"g: 8-bit Greyscale, Binary output\n"
			"c: 24-bit colour, Binary output\n"
			"\n"
			"-s S: steps between adjacent colours, default 11\n"
			"-i I: Iterations for mandelbrot recursion, default 255\n"
			"\n"
			"-p: Point to zoom in on\n"
			"X Y: Point to zoom in on\n"
			"-r R: radius of BOUNDing box around zoomed point, too low values can cause rounding errors\n"
			"-p X Y -r R will draw bounding box [X-R,Y-R] to [X+R,Y+R]\n"
			"Will default to drawing bounding box about\n"
			"\n"
			);
	return;
}

void print_usage()
{
	printf("usage: mandelbrot [-h | --help] [-d W H] [-a m|j] [-j X Y] [-f b|g|c] [-s S] [-i I] [-p X Y] [-r R]\n");
	return;
}

int main(int argc, char *argv[])
{
	char format = 'g'; /* Default greyscale */
	char algorithm = 'm'; /* Default mandelbrot */
	int i;

	for (i = 1; i < argc; i++) {
		/* print halp */
		if (input_eq("-h") || input_eq("--help")) {
			print_help();
			return 0;
		}
		/* select algorithm */
		else if (input_eq("-a")) {
			sscanf(argv[i + 1], "%c", &algorithm);
			i++;
		}
		/* read value of c = X + Yi for the julia set */
		else if (input_eq("-j")) { 
			sscanf(argv[i + 1], "%lf", &JULIA_X);
			sscanf(argv[i + 2], "%lf", &JULIA_Y);
			i += 2;
		}
		/* read dimension */
		else if (input_eq("-d")) { 
			sscanf(argv[i + 1], "%d", &WIDTH);
			sscanf(argv[i + 2], "%d", &HEIGHT);
			i += 2;
		}
		/* read format */
		else if (input_eq("-f")) {
			sscanf(argv[i + 1], "%c", &format);
			i++;
		}
		/* read color step */
		else if (input_eq("-s")) { 
			sscanf(argv[i + 1], "%d", &COLOR_STEP);
			i++;
		}
		/* read iter */
		else if (input_eq("-i")) { 
			sscanf(argv[i + 1], "%d", &ITER_MAX);
			i++;
		}
		/* read zoom */
		else if (input_eq("-p")) { 
			sscanf(argv[i + 1], "%lf", &CENTER_X);
			sscanf(argv[i + 2], "%lf", &CENTER_Y);
			i += 2;
		}
		/* read radius */
		else if (input_eq("-r")) {
			sscanf(argv[i + 1], "%lf", &RADIUS);
			i++;
		} else {
			/* error, did not match */
			print_usage();
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

