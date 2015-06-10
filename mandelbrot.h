#include <stdio.h>
#include <stdlib.h>
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

void mandelbrot_bw(int width, int height, double center_x, double center_y, double radius, double bound, int iter_max, int color_step);
void mandelbrot_gs(int width, int height, double center_x, double center_y, double radius, double bound, int iter_max, int color_step);
void mandelbrot_col(int width, int height, double center_x, double center_y, double radius, double bound, int iter_max, int color_step);

