#include "fractal.h"

int main(int argc, char *argv[])
{
	char format = 'g'; /* Default greyscale */
	char algorithm = 'm'; /* Default mandelbrot */
	char *image = 0;
	int i;
	size_t len = 0;
	struct mandelparams p;
	p.width = 1024;
	p.height = 1024;
	p.center_x = 0;
	p.center_y = 0;
	p.scale_factor_x = 2 * 2 / (double)1024 + 1;
	p.scale_factor_y = 2 * 2 / (double)1024 + 1;
	p.radius = 2;
	p.bound = 4;
	p.iter_max = 100;
	p.color_step = 11;
	double julia_x = 0.3515;
	double julia_y = 0.42193;

	if (argc == 1) {
		print_usage();
		return 0;
	}
	for (i = 1; i < argc; i++) {
		/* print halp */
		if (input_eq("-h") || input_eq("--help")) {
			print_help();
			return 0;
		}
		/* read window size */
		else if (input_eq("-window")) { 
			sscanf(argv[i + 1], "%d", &p.width);
			sscanf(argv[i + 2], "%d", &p.height);
			i += 2;
		}
		/* select algorithm */
		else if (input_eq("-mandel")) {
			algorithm = 'm';
		}
		/* read value of c = X + Yi for the julia set */
		else if (input_eq("-julia")) {
			algorithm = 'j'; 
			sscanf(argv[i + 1], "%lf", &julia_x);
			sscanf(argv[i + 2], "%lf", &julia_y);
			i += 2;
		}
		/* read format */
		else if (input_eq("-bw")) {
			format = 'b';
		}
		else if (input_eq("-grey")) {
			format = 'g';
		}
		else if (input_eq("-color")) {
			format = 'c';
		}
		else if (input_eq("-bmp")) {
			format = 't';
		}
		/* read color step */
		else if (input_eq("-color-step")) {
			format = 'c';
			sscanf(argv[i + 1], "%d", &p.color_step);
			i++;
		}	
		/* read iter */
		else if (input_eq("-iter")) { 
			sscanf(argv[i + 1], "%d", &p.iter_max);
			i++;
		}
		/* read origin */
		else if (input_eq("-origin")) { 
			sscanf(argv[i + 1], "%lf", &p.center_x);
			sscanf(argv[i + 2], "%lf", &p.center_y);
			i += 2;
		}
		/* read radius */
		else if (input_eq("-radius")) {
			sscanf(argv[i + 1], "%lf", &p.radius);
			i++;
		} else {
			/* error, did not match */
			printf("Cannot parse args.\n");
			return 1;
		}
	}

	p.scale_factor_x = 2 * p.radius / (double)p.width + 1;
	p.scale_factor_y = 2 * p.radius / (double)p.height + 1;

	if (algorithm == 'j') {
		image = julia_col(p, julia_x, julia_y);
		len = (p.width * p.height * sizeof(color) + 9 + log10(p.width) + log10(p.height));
	} else {
		switch (format) {
		case 'b':
			image = mandelbrot_bw(p);
			len = (p.width * p.height * sizeof(char) * 2 + p.height + 5 + log10(p.width) + log10(p.height));
			break;
		case 'g':
			image = mandelbrot_gs(p);
			len = (p.width * (p.height + 1) * sizeof(char) + 10 + log10(p.width) + log10(p.height));
			break;
		case 'c':
			image = mandelbrot_col(p);
			len = (p.width * (p.height + 1) * sizeof(color) + 10 + log10(p.width) + log10(p.height));
			break;
		case 't':
			image = mandelbrot_bmp(p);
			int padding = (4 - ((p.width * 3) % 4)) % 4;
			len = p.height * ((p.width * 3) + padding) + 26;
			break;
		}
	}

	write(1, image, len);
	free(image);

	return 0;
}

void print_usage()
{
	printf("usage: mandelbrot [-h | -help] [-window width height] " 
			"[-mandel] [-julia X Y] [-bw | -grey | -color | -color-step STEP] "
			"[-iter I] [-origin X Y] [-radius R] > file.ppm\n");
	return;
}

void print_help()
{
	printf("usage: mandelbrot [-h | -help] [-window width height]" 
			"[-mandel] [-julia X Y] [-bw | -grey | -color | -color-step STEP]"
			"[-iter I] [-origin X Y] [-radius R] > file.ppm\n"
			"-h | --help: Print this help\n"
			"-window width height: dimensions of image, default 1024x1024\n"
			"-mandel: Draw the mandelbrot set\n"
			"-julia X Y: Draw the Julia set with initial value C = X + Yi, default C = -0.778 - 0.116i \n"
			"-bw: 1-bit B&W, ASCII output\n"
			"-grey: 8-bit Greyscale, Binary output\n"
			"-color: 24-bit colour, Binary output\n"
			"-color-step STEP: 24-bit colour, Binary output, manual colour step, default 11\n"
			"-iter I: Iterations for mandelbrot recursion, default 100\n"
			"-origin X Y: Location to center output image about\n"
			"-radius R: Output image will be the bounding box [X-R,Y-R] to [X+R,Y+R]\n"
			"Will default to drawing bounding box [-1,-1] to [1,1]\n"
			"Pipe the result into file.ppm\n"
	      );
	return;
}

