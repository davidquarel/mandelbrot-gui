#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef DCOMPLEX_H
#include "dcomplex.h"
#define DCOMPLEX_H
#endif

#ifndef COLOR_H
#include "color.h"
#define COLOR_H
#endif

char *julia_col(int width, int height, double julia_x, double julia_y);

