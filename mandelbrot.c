/*
Mandelbrot Set
David Quarel 07/06/15
Pipe output from code into a .ppm file
*/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

typedef struct{
	float re;
	float im;
} complex;

int width;
int height;

//Functions for Complex Numbers
complex* make_complex(float real, float imag){
	complex new_complex = {real, imag};
	return new_complex;
}

complex add(complex a, complex b){
	return make_complex(a.re + b.re, a.im + b.im);
}

complex mult(complex a, complex b){
	return make_complex(a.re*b.re - a.im*b.im , a.re*b.im + a.im*b.re);
}

float mag_sq(complex a){
	return (a.re*a.re + a.im*a.im);
}

complex coord_to_complex(int x, int y){
	float new_x;
	float new_y;
	new_x = (float)x * 3 / width - 2; //scale [0,width] to [-2,1]  
	new_y = (float)y * 2 / height - 1; //scale [0,height] to [-1,1]
	return make_complex(new_x,new_y);
}

#define c_zero make_complex(0,0);
/*
complex div(complex a, complex b){
	float scale = 1/(mag_sq(b));
	return make_complex(scale*(a.re*b.re + a.im*b.im) , scale*(a.im*b.re - a.re*b.im));
}

complex scale(float s, complex a){
	return make_complex(s*a.re,s*a.im);
}

void print_complex(complex a){
	printf("%f + %fi",a.re,a.im);
	return;
}
*/

const float bound = 4; //square roots are expensive

int iter_max = 255;

void mandelbrot_bw(){ //0 if in set, 1 else, B&W
	printf("P1\n%d %d\n",width,height);
	int x;
	int y;
	complex c;

	for(y=0; y < height; y++){ 
		for(x=0; x < width; x++){
			c = coord_to_complex(x,y);
			int iter;

			complex z = c_zero;

			for(iter=0; iter<iter_max; iter++){
				if(mag_sq(z) >= bound){
					printf("0 "); //point escaped bounds, outside set
					break;
				}
				z = add(mult(z,z),c);  //iterate again, z_{i+1} = z_i^2 + c
			}
			if(iter == iter_max){
				printf("1 "); //point stayed in set after ITER_LIMIT_BW iterations, in set
			}

		}
		printf("\n");
	}
	return;
}
//Bitshifting witchcraft
void push_bit(char n) {
   static int offset = 8;
   static char out = 0;
   offset--;
   out |= n<<offset;
   if (offset == 0) {
   	offset = 8;
   	putchar(out);
   	out = 0;
   }

}

void mandelbrot_bw_b(){ //0 if in set, 1 else, B&W
	printf("P4\n%d %d\n",width,height);
	int x;
	int y;
	complex c;

	for(y=0; y < height; y++){ 
		for(x=0; x < width; x++){
			c = coord_to_complex(x,y);
			int iter;

			complex z = c_zero;

			for(iter=0; iter<iter_max; iter++){
				if(mag_sq(z) >= bound){
					push_bit(0); //point escaped bounds, outside set
					break;
				}
				z = add(mult(z,z),c);  //iterate again, z_{i+1} = z_i^2 + c
			}
			if(iter == iter_max){
				push_bit(1); //point stayed in set after ITER_LIMIT_BW iterations, in set
			}

		}
		
	}
	return;
}

#define ITER_LIMIT_8 255

void mandelbrot_gs(){ //8-bit greyscale ASCII
	printf("P2\n%d %d\n#Mandelbrot Set 8-bit Greyscale\n255\n",width,height);
	int x;
	int y;
	complex c;
	for(y=0; y < height; y++){ 
		for(x=0; x < width; x++){
			c = coord_to_complex(x,y);
			int iter;

			complex z = c_zero;

			for(iter=ITER_LIMIT_8; iter>0; iter--){
				if(mag_sq(z) >= bound){
					printf("%3d ",iter);
					break;
				}
				z = add(mult(z,z),c);  //z_{i+1} = z_i^2 + c
			}
			if(!iter){
				printf("%3d ",0);
			}
		}
		printf("\n");
	}
	return;
}

void mandelbrot_gs_b(){ //8-bit binary greyscale binary
	printf("P5\n%d %d\n255\n",width,height);
	int x;
	int y;
	complex c;
	for(y=0; y < height; y++){ 
		for(x=0; x < width; x++){
			c = coord_to_complex(x,y);
			int iter;

			complex z = c_zero;

			for(iter=ITER_LIMIT_8; iter>0; iter--){
				if(mag_sq(z) >= bound){
					putchar(iter);
					break;
				}
				z = add(mult(z,z),c);  //z_{i+1} = z_i^2 + c
			}
			if(!iter){
				putchar(0);
			}
		}
	}
	return;
}

typedef struct{
	char r;
	char g;
	char b;
} color;

color make_color(int red, int green, int blue){
	color new_color = {red, green, blue};
	return new_color;
}

void mandelbrot_col(){ //24-bit, colour changes with iteration depth, binary
	color world[width*height];
	printf("P6\n%d %d\n255\n",width,height); //P6, colour binary file
	int x;
	int y;
	complex c;
	int iter_max_col;
	for(iter_max_col=1; iter_max_col<35; iter_max_col++){
		for(y=0; y < height; y++){ 
			for(x=0; x < width; x++){
				c = coord_to_complex(x,y);
				complex z = c_zero;
				int iter;
				for(iter=iter_max_col; iter>0; iter--){
					if(mag_sq(z) >= bound){
						break;
					}
					z = add(mult(z,z),c);  //z_{i+1} = z_i^2 + c
				}
				if(!iter){
					char val = iter_max_col*11; //choose prime numbers for pretty colours
					world[width*y+x] = make_color(val/2,val/2,val);
				}
			}
		}
	}

	for(y=0; y < height; y++){ 
		for(x=0; x < width; x++){
			color cell = world[width*y+x];
			printf("%c%c%c",cell.r,cell.g,cell.b);
		}
	}

	return;

}

void print_help(){
	printf("usage: mandelbrot W H b|w|g|x [i]\n"
		"W: Width of picture to generate\n"
		"H: Height of picture to generate\n"
		"b: 1-bit B&W, ASCII output\n"
		"w: 1-bit B&W, Binary output\n"
		"i: Iterations for 1-bit B&W, default 255\n"
		"g: 8-bit Greyscale, ASCII output\n"
		"x: 8-bit Greyscale, Binary output\n"
		"c: 24-bit colour, Binary output\n"
		);
	return;
}

void print_usage(){
	printf("usage: mandelbrot W H b|w|g|x [i]\n");
	return;
}

int main(int argc, char *argv[]){
    char format;
    if(!strcmp(argv[1],"-h") || !strcmp(argv[1],"--help")){
    	print_help();
    	return 0;
    } 

    if(argc == 4){
		sscanf(argv[1],"%d",&width); //read width
	    sscanf(argv[2],"%d",&height); //read height
	    sscanf(argv[3],"%c",&format); //read format
	}

	else if(argc == 5){
		sscanf(argv[1],"%d",&width); //read width
	    sscanf(argv[2],"%d",&height); //read height
	    sscanf(argv[3],"%c",&format); //read format
	    sscanf(argv[4],"%d",&iter_max); //read iterations for black and white
	}

	else{
	    print_usage();
	    return 1;
	}
    
    //End Handle Input

	if(format == 'b'){ //1-bit black and white mandelbrot
		mandelbrot_bw();
	}

	else if(format == 'g'){ //8-bit greyscale mandelbrot
		mandelbrot_gs();	
	}

	else if(format == 'x'){ //8-bit greyscale binary mandelbrot
		mandelbrot_gs_b();
	}
	else if(format == 'w'){ //1-bit black and white mandelbrot binary
		mandelbrot_bw_b();
	}

	else if(format == 'c'){ //24-bit colour, changes colour with iteration depth
		mandelbrot_col();
	}
	
	return 0;
}

