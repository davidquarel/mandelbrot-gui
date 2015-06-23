#include "julia.h"

char *julia_col(struct mandelparams p, int julia_x, int julia_y)
{
	complex julia_c = make_complex(julia_x, julia_y);
	char *image = (char *)malloc(p.height * p.width * sizeof(color) + 50);
	int pre = sprintf(image, "P6\n%d %d\n255\n", p.width, p.height); /* P6, colour binary file */
	color *world = (color *)(image + pre);
	color *current = world;
	int x;
	int y;
	complex z;
	#pragma omp parallel private(x, y, z, current)
	{
		for (y = 0; y < p.height; y++) {
			#pragma omp for
			for (x = 0; x < p.width; x++) {
				current = world + (y * p.width) + x;
				z = coord_to_complex(x, y, p.width, p.height, p.center_x, p.center_y, p.radius);
				int iter;
				for (iter = p.iter_max; iter > 0; iter--) {
					if (mag_sq(z) >= p.bound) {
						char val = iter * p.color_step;
						*current = make_color(val, val, val*2);
						break;
					}
					z = add(square(z), julia_c);  /* z_{i+1} = z_i^2 + c */
				}
				if (!iter) {
					*current = make_color(0, 0, 0);
				}
			}
		}
	}

	return image;
}

