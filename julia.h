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

void julia_col(int width,
	       int height,
	       double center_x,
	       double center_y,
	       double radius,
	       double bound,
	       int iter_max,
	       int color_step,
	       double julia_x,
	       double julia_y);

