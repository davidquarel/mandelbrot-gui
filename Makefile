CFLAGS=-O2 -Wall -Werror --std=c99 -Wno-unused-result

default: all

all: fractal

fractal: fractal.c fractal.h mandelbrot.o julia.o dcomplex.o color.o
	gcc fractal.c mandelbrot.o julia.o dcomplex.o color.o $(CFLAGS) -fopenmp -lm -o fractal

julia.o: julia.c julia.h
	gcc julia.c -c -o julia.o

mandelbrot.o: mandelbrot.c
	gcc mandelbrot.c $(CFLAGS) -fopenmp -c -o mandelbrot.o

dcomplex.o: dcomplex.c dcomplex.h
	gcc dcomplex.c $(CFLAGS) -c -o dcomplex.o

color.o: color.c color.h
	gcc color.c $(CFLAGS) -c -o color.o

clean:
	rm -f mandelbrot dcomplex.o julia.o mandelbrot.o color.o

