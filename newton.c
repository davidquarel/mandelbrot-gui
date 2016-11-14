/*
    How to compile

    g++ newton.c -w -lSDL2 -o newton
    ./newton

*/


#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <complex.h>
#include <SDL2/SDL.h>
#include <math.h>

#define PI 3.141592

double complex newton(double complex x){
    return x - (cpow(x,3) - 1)/(3*cpow(x,2)); // x - \frac{x^3 - 1}{3x^2}
}   

void refresh_fractal()
{

for (int x = 0; x < SIZE; ++x)
    {
        for (int y = 0; y < SIZE; ++y)
        {
            prevz = 9999;
            z = ORIGIN + x*DELTA + y*DELTA*I;
            for (i = 0; i < ITER; ++i)
            {
                if( cabs(prevz - z) < EPSILON )
                    break;
                prevz = z;  
                z = newton(z);
                
            }
            //false colouring
            color = i%16 * 16;

            if(cabs (z - 1) < 0.001)
            {
                red = color;
                green = 0;
                blue = 0;
            }
            else if(cabs (z - cexp(I * 2*PI/3)) < 0.001)
            {
                red = 0;
                green = color;
                blue = 0;
            }
            else{
                red = 0;
                green = 0;
                blue = color;
            }



            SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }

    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface( window );

}

int main(int argc, char *argv[]){  
    
    int SIZE = 800;
    int ITER = 64;
    double EPSILON = 0.000001; //10E-10

    double complex CENTER = 0 + 0*I;
    double RADIUS = 2;
    double DELTA = (2 * RADIUS) / SIZE;
    double complex ORIGIN = CENTER - (1+I)*RADIUS;
    double complex z;
    double complex prevz;

    uint8_t image[SIZE][SIZE];
    int i;

    //i dunno how SDL works
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(SIZE, SIZE, 0, &window, &renderer);

    uint8_t color;
    uint8_t red;
    uint8_t blue;
    uint8_t green;

    uint16_t xmouse;
    uint16_t ymouse;

    refresh_fractal();

    //magic make the window quit code
    bool quit = false;
    SDL_Event e;

    while( !quit )
    {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
                quit = true;
            else if(e.type == SDL_MOUSEBUTTONDOWN)
            {
                RADIUS = RADIUS*0.75;
                SDL_GetMouseState(&xmouse, &ymouse);
                CENTER = 
            }
        }
    }
    SDL_DestroyWindow( window );
    window = NULL;
    SDL_Quit();

    return 0;
}