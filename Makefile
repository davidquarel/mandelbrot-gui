default: all

all: mandelbrot

mandelbrot: mandelbrot.c
	gcc mandelbrot.c -O2 --std=c99 -fopenmp -o mandelbrot

clean:
	rm mandelbrot

