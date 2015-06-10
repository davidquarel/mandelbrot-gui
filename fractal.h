/* BENCHMARK
 * time ./mandelbrot -d 8192 8192 -f c -i 150 -z -0.16 1.033 0.015 > test.ppm
 * this took partch 3m16s
 */

#include <string.h>
#include <math.h>

#include "julia.h"
#include "mandelbrot.h"

#define input_eq(s1) !(strcmp(argv[i],s1))

const double bound = 4; /* square roots are expensive */
int iter_max = 100;
int color_step = 11;

int width = 1024;
int height = 1024;
double center_x = 0;
double center_y = 0;
double radius = 1;

double julia_x = 0.3515;
double julia_y = 0.42193;

double scale_factor_x = 2 * 2 / (double)1024 + 1;
double scale_factor_y = 2 * 2 / (double)1024 + 1;

void print_usage();
void print_help();

