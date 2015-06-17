typedef struct {
	double re;
	double im;
} complex;

complex make_complex(double real, double imag);
complex coord_to_complex(int x, int y, double width, double height, double center_x, double center_y, double radius);
complex add(complex a, complex b);
complex mult(complex a, complex b);
complex square(complex a);
double mag_sq(complex a);

