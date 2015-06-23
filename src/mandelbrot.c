/* Mandelbrot Set
 * David Quarel 07/06/15
 * Pipe output from code into a .ppm file
 * Got zoom working
 * Color is now parallelized
 */

#include "mandelbrot.h"

char bmpheader[] = "BM"			/* size - 26 bytes */
     		   "\x00\x00\x00\x00"
     		   "\xDE\xAD\xBE\xEF"
		   "\x1A\x00\x00\x00"
		   "\x0C\x00\x00\x00"
		   "\x02\x00\x02\x00"
		   "\x01\x00\x18\x00";

char *mandelbrot_bw(int width, int height)
{
	char *image = (char *)malloc(width * height * sizeof(char) * 2 + height + 16);
	char *start = image + sprintf(image, "P1\n%d %d\n", width, height);
	char *current;
	extern double center_x;
	extern double center_y;
	extern double radius;
	extern double bound;
	extern int iter_max;
	int x;
	int y;
	complex c;
	#pragma omp parallel private(x, y, c, current) shared(image)
	{
		#pragma omp for
		for (y = 0; y < height; y++) {
			current = start + (y * width * 2 + y);
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
					z = add(square(z), c);  /* iterate again, z_{i+1} = z_i^2 + c */
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
	return image;
}

/* 8-bit binary greyscale binary */
char *mandelbrot_gs(int width, int height)
{
	char *image = (char *)malloc(width * height * sizeof(char) + 50);
	int pre = sprintf(image, "P5\n%d %d\n255\n", width, height);
	char *start = image + pre;
	char *current;
	extern double center_x;
	extern double center_y;
	extern double radius;
	extern double bound;
	extern int iter_max;
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
					z = add(square(z), c);  /* z_{i+1} = z_i^2 + c */
				}
				if (!iter) {
					/* putchar(0); */
					*(current++) = 0;
				}
			}
		}
	}
	return image;
}

/* 24-bit, colour changes with iteration depth, binary */
char *mandelbrot_col(int width, int height)
{
	char *image = (char *)malloc(height * width * sizeof(color) + 50);
	int pre = sprintf(image, "P6\n%d %d\n255\n", width, height); /* P6, colour binary file */
	color *world = (color *)(image + pre);
	color *current = world;
	extern double center_x;
	extern double center_y;
	extern double radius;
	extern double bound;
	extern int iter_max;
	extern int color_step;
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
						*current = make_color(val, val, val*2);
						break;
					}
					z = add(square(z), c);  /* z_{i+1} = z_i^2 + c */
				}
				if (!iter) {
					*current = make_color(0, 0, 0);
				}
			}
		}
	}
	return image;
}

/* 24-bit bmp output */
char *mandelbrot_bmp(int width, int height)
{
	int padding = (4 - ((width * 3) % 4)) % 4;
	int size = height * ((width * 3) + padding) + 26;
	fprintf(stderr, "width: %d height: %d padding: %d size: %d\n", width, height, padding, size);
	char *image = (char *)calloc(size, 1);
	memcpy(image, bmpheader, 26); /* copy in the bmp file header */
	*(int *)(image + 2) = size;	      /* this is very not portable */
	*(int *)(image + 18) = width;
	*(int *)(image + 20) = height;
	color *world = (color *)(image + 26);
	color *current = world;
	extern double center_x;
	extern double center_y;
	extern double radius;
	extern double bound;
	extern int iter_max;
	extern int color_step;
	int x;
	int y;
	complex c;
	complex z;
	#pragma omp parallel private(x, y, c, z, current)
	{
		for (y = 0; y < height; y++) {
			#pragma omp for
			for (x = 0; x < width; x++) {
				//current = world + (y * (width + padding)) + x;
				current = (color *)(((char *)(world + (y * width) + x)) + (padding * y));
				c = coord_to_complex(x, y, width, height, center_x, center_y, radius);
				z = make_complex(0, 0);
				int iter;
				for (iter = iter_max; iter > 0; iter--) {
					if (mag_sq(z) >= bound) {
						char val = iter * color_step;
						*current = make_color(val * 2, val, val);
						break;
					}
					z = add(square(z), c);  /* z_{i+1} = z_i^2 + c */
				}
				if (!iter) {
					*current = make_color(0, 0, 0);
				}
			}
		}
	}
	return image;
}


