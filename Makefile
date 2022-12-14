CFLAGS=-O2 -Wall -Werror --std=c99 -Wno-unused-result

default: all

all: bin/fractal

remake: clean bin/fractal

bin/fractal: src/fractal.c src/fractal.h bin/mandelbrot.o bin/julia.o bin/dcomplex.o bin/color.o
	gcc src/fractal.c bin/mandelbrot.o bin/julia.o bin/dcomplex.o bin/color.o $(CFLAGS) -fopenmp -lm -o bin/fractal

bin/julia.o: src/julia.c src/julia.h
	gcc src/julia.c $(CFLAGS) -fopenmp -c -o bin/julia.o

bin/mandelbrot.o: src/mandelbrot.c
	gcc src/mandelbrot.c $(CFLAGS) -fopenmp -c -o bin/mandelbrot.o

bin/dcomplex.o: src/dcomplex.c src/dcomplex.h
	gcc src/dcomplex.c $(CFLAGS) -c -o bin/dcomplex.o

bin/color.o: src/color.c src/color.h
	gcc src/color.c $(CFLAGS) -c -o bin/color.o

clean:
	rm -f bin/*

