/* Mandelbrot Set
 * David Quarel 07/06/15
 * Pipe output from code into a .ppm file
 * Got zoom working
 * Color is now parallelized
 */

#include "mandelbrot.h"

void mandelbrot_bw(int width,
		   int height,
		   double center_x,
		   double center_y,
		   double radius,
		   double bound,
		   int iter_max,
		   int color_step)
{
	char *image = (char *)malloc(width * height * sizeof(char) * 2 + height + 16);
	char *start = image + sprintf(image, "P1\n%d %d\n", width, height);
	char *current;

	int x;
	int y;
	complex c;
	#pragma omp parallel private(x, y, c, current) shared(image)
	{
		#pragma omp for
		for (y = 0; y < height; y++) {
			current = start + (y * height * 2 + y);
			for (x = 0; x < width; x++) {
				c = coord_to_complex(x, y, width, height, center_x, center_y, radius);
				int iter;

				complex z = make_complex(0, 0);

				for (iter = iter_max; iter > 0; iter--) {
					if (mag_sq(z) >= bound) {
						/* point escaped bounds */
						*(current++) = '0';
						break;
					}
					z = add(mult(z, z), c);  /* iterate again, z_{i+1} = z_i^2 + c */
				}
				if (!iter) {
					/* printf("1 "); //point stayed in set after ITER_LIMIT_BW iterations, in set */
					*(current++) = '1';
				}
				*(current++) = ' ';
			}
			*current = '\n';
		}
	}
	puts(image);
	free(image);
	return;
}

/* 8-bit binary greyscale binary */
void mandelbrot_gs(int width,
		   int height,
		   double center_x,
		   double center_y,
		   double radius,
		   double bound,
		   int iter_max,
		   int color_step)
{
	char *image = (char *)malloc(width * height * sizeof(char) + 50);
	int pre = sprintf(image, "P5\n%d %d\n255\n", width, height);

	char *start = image + pre;
	char *current;

	int x;
	int y;
	complex c;
	#pragma omp parallel private(x, y, c, current)
	{ 
		#pragma omp for
		for (y = 0; y < height; y++) {
			current = start + (y * height);
			for (x = 0; x < width; x++) {
				c = coord_to_complex(x, y, width, height, center_x, center_y, radius);
				int iter;

				complex z = make_complex(0, 0);

				for (iter = iter_max; iter > 0; iter--) {
					if (mag_sq(z) >= bound) {
						/* putchar(iter); */
						*(current++) = iter;
						break;
					}
					z = add(mult(z, z), c);  /* z_{i+1} = z_i^2 + c */
				}
				if (!iter) {
					/* putchar(0); */
					*(current++) = 0;
				}
			}
		}
	}
	write(1, image, pre + height * width);
	free(image);
	return;
}

/* 24-bit, colour changes with iteration depth, binary */
void mandelbrot_col(int width,
		    int height,
		    double center_x,
		    double center_y,
		    double radius,
		    double bound,
		    int iter_max,
		    int color_step)
{
	char *image = (char *)malloc(height * width * sizeof(color) + 50);
	int pre = sprintf(image, "P6\n%d %d\n255\n", width, height); /* P6, colour binary file */
	color *world = (color *)(image + pre);
	color *current = world;
	int x;
	int y;
	complex c;
	complex z;
	#pragma omp parallel private(x, y, c, z, current)
	{
		for (y = 0; y < height; y++) {
			#pragma omp for
			for (x = 0; x < width; x++) {
				current = world + (y * width) + x;
				c = coord_to_complex(x, y, width, height, center_x, center_y, radius);
				z = make_complex(0, 0);
				int iter;
				for (iter = iter_max; iter > 0; iter--) {
					if (mag_sq(z) >= bound) {
						char val = iter * color_step;
						*current = make_color(val*2, val, val);
						break;
					}
					z = add(mult(z, z), c);  /* z_{i+1} = z_i^2 + c */
				}
				if (!iter) {
					*current = make_color(0, 0, 0);
				}
			}
		}
	}
	write(1, image, height * width * sizeof(color) + pre);
	free(image);

	return;
}

