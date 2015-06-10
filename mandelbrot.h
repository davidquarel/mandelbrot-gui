#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#ifndef DCOMPLEX_H
#include "dcomplex.h"
#define DCOMPLEX_H
#endif

#ifndef COLOR_H
#include "color.h"
#define COLOR_H
#endif

#include "unistd.h"

char *mandelbrot_bw(int width, int height, double center_x, double center_y, double radius, double bound, int iter_max, int color_step);
char *mandelbrot_gs(int width, int height, double center_x, double center_y, double radius, double bound, int iter_max, int color_step);
char *mandelbrot_col(int width, int height, double center_x, double center_y, double radius, double bound, int iter_max, int color_step);

