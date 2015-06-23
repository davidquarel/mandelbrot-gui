/* BENCHMARK
 * time ./mandelbrot -d 8192 8192 -f c -i 150 -z -0.16 1.033 0.015 > test.ppm
 * this took partch 3m16s
 */

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

#ifndef MANDELBROT_H
#define MANDELBROT_H
#include "mandelbrot.h"
#endif

#ifndef JULIA_H
#define JULIA_H
#include "julia.h"
#endif

void print_usage();
void print_help();

