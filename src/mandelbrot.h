#ifndef MANDELBROT_H
#define MANDELBROT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "dcomplex.h"
#include "color.h"
#include "fractal.h"
#include "unistd.h"

char *mandelbrot_bw(struct mandelparams p);
char *mandelbrot_gs(struct mandelparams p);
char *mandelbrot_col(struct mandelparams p);
char *mandelbrot_bmp(struct mandelparams p);
#endif

