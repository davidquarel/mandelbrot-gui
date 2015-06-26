#ifndef COLOR_H
#define COLOR_H
typedef struct {
	char r;
	char g;
	char b;
} color;

color make_color(int red, int green, int blue);
#endif

