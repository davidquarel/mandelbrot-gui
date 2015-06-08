CFLAGS=-O2 -Wall -Werror --std=c99 -fopenmp

default: all

all: mandelbrot

mandelbrot: mandelbrot.c
	gcc mandelbrot.c $(CFLAGS) -o mandelbrot

clean:
	rm mandelbrot

