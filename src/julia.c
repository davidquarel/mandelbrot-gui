#include "julia.h"

char *julia_col(int width, int height, double julia_x, double julia_y)
{
	complex julia_c = make_complex(julia_x, julia_y);
	char *image = (char *)malloc(height * width * sizeof(color) + 50);
	int pre = sprintf(image, "P6\n%d %d\n255\n", width, height); /* P6, colour binary file */
	extern double center_x;
	extern double center_y;
	extern double radius;
	extern double bound;
	extern int iter_max;
	extern int color_step;
	color *world = (color *)(image + pre);
	color *current = world;
	int x;
	int y;
	complex z;
	#pragma omp parallel private(x, y, z, current)
	{
		for (y = 0; y < height; y++) {
			#pragma omp for
			for (x = 0; x < width; x++) {
				current = world + (y * width) + x;
				z = coord_to_complex(x, y, width, height, center_x, center_y, radius);
				int iter;
				for (iter = iter_max; iter > 0; iter--) {
					if (mag_sq(z) >= bound) {
						char val = iter * color_step;
						*current = make_color(val, val, val*2);
						break;
					}
					z = add(mult(z, z), julia_c);  /* z_{i+1} = z_i^2 + c */
				}
				if (!iter) {
					*current = make_color(0, 0, 0);
				}
			}
		}
	}

	return image;
}

