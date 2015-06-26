#ifndef FRACTAL_H
#define FRACTAL_H
#include <string.h>
#include <math.h>


#define input_eq(s1) !(strcmp(argv[i],s1))

struct mandelparams {
	int width;
	int height;
	double center_x;
	double center_y;
	double radius;
	double bound;
	int iter_max;
	int color_step;
	double scale_factor_x;
	double scale_factor_y;
};

#include "mandelbrot.h"
#include "julia.h"

void print_usage();
void print_help();
#endif

