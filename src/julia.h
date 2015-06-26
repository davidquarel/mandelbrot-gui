#ifndef JULIA_H
#define JULIA_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "dcomplex.h"
#include "color.h"
#include "fractal.h"

char *julia_col(struct mandelparams p, int julia_x, int julia_y);
#endif

