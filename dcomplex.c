#ifndef DCOMPLEX_H
#include "dcomplex.h"
#define DCOMPLEX_H
#endif

complex make_complex(double real, double imag)
{
	complex new_complex = {real, imag};
	return new_complex;
}

complex coord_to_complex(int x,
			 int y,
			 double width,
			 double height,
			 double center_x,
			 double center_y,
			 double radius)
{
	double new_x;
	double new_y;
	new_x = (double)x * 2 * radius / (double)width + center_x - radius; //scale [0,SIZE] to [X-R,X+R]
	new_y = (double)y * 2 * radius / (double)height - center_y - radius; //scale [0,SIZE] to [Y-R,Y+R]
	return make_complex(new_x, new_y);
}

complex add(complex a, complex b)
{
	return make_complex(a.re + b.re, a.im + b.im);
}

complex mult(complex a, complex b)
{
	return make_complex(a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re);
}

double mag_sq(complex a)
{
	return (a.re * a.re + a.im * a.im);
}

