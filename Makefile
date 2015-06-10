CFLAGS=-O2 -Wall -Werror --std=c99 -Wno-unused-result

default: all

all: mandelbrot

mandelbrot: mandelbrot.c dcomplex.o
	gcc mandelbrot.c dcomplex.o $(CFLAGS) -fopenmp -o mandelbrot

dcomplex.o: dcomplex.c dcomplex.h
	gcc dcomplex.c $(CFLAGS) -c -o dcomplex.o

clean:
	rm mandelbrot dcomplex.o

