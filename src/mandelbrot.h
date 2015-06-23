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

#ifndef FRACTAL_H
#define FRACTAL_H
#include "fractal.h"
#endif

#include "unistd.h"

char *mandelbrot_bw(struct mandelparams p);
char *mandelbrot_gs(struct mandelparams p);
char *mandelbrot_col(struct mandelparams p);
char *mandelbrot_bmp(struct mandelparams p);

