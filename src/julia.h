#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef DCOMPLEX_H
#define DCOMPLEX_H
#include "dcomplex.h"
#endif

#ifndef COLOR_H
#define COLOR_H
#include "color.h"
#endif

#ifndef FRACTAL_H
#define FRACTAL_H
#include "fractal.h"
#endif

char *julia_col(struct mandelparams p, int julia_x, int julia_y);

