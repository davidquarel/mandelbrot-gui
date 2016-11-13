#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <complex.h>

#pragma pack(push)
#pragma pack(1)
    typedef struct{
        char blue;
        char green;
        char red;
    }pixel;
#pragma pack(pop)


#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */
    struct dib_header { //struct takes up exactly 40 bytes
    uint32_t size; //should be 40 bytes
    int32_t width;  //signed
    int32_t height; //signed
    uint16_t color_planes; //must be 1
    uint16_t bits_per_pixel; //usually 24 bits/pixel
    uint32_t compression_method; //0 for no compression
    uint32_t image_size; //size of raw bitmap data
    int32_t hor_res; //pixels per meter, horizontal resolution
    int32_t vert_res; //pixels per meter, vertical resolution
    uint32_t num_of_colors; //0 to default 2**n
    uint32_t num_important_colors; //0 when all important, usually ignored
};
#pragma pack(pop)



#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */
struct bmp_header{ //struct takes up exactly 14 bytes
    char bm[2]; //must contain BM
    uint32_t filesize; //number of bytes in entire bmp file
    uint16_t reserved_1; //unused
    uint16_t reserved_2; //unused
    uint32_t pixel_array_offset; //where to offset reading the pixel data
};
#pragma pack(pop)



double complex newton(double complex x){
    return x - (cpow(x,3) - 1)/(3*cpow(x,2)); // x - \frac{x^3 - 1}{3x^2}
}   

int main(int argc, char *argv[]){  
    
    int SIZE = 1024;
    int ITER = 128;
    double EPSILON = 0.00001;

    double complex CENTER = 0 + 0*I;
    double RADIUS = 2;
    double DELTA = (2 * RADIUS) / SIZE;
    double ORIGIN = CENTER - RADIUS - RADIUS*I;
    double complex z;
    double complex prevz;

    uint8_t image[SIZE][SIZE];
    int i;
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
            image[x][y] = i;
        }
    }

    //save image to file
    FILE *file_out;
    file_out = fopen("newton.bmp","wb");
    struct bmp_header bmp;
    struct dib_header dib;
    uint16_t xsize = SIZE;
    uint16_t ysize = SIZE;
    uint32_t NUM_PIXELS = xsize*ysize;
    uint8_t BMP_HEADER_SIZE = 14;
    uint8_t DIB_HEADER_SIZE = 40;
    uint8_t BYTES_PER_PIXEL = 3;
    bmp.bm[0] = 'B';
    bmp.bm[1] = 'M';
    bmp.filesize = BMP_HEADER_SIZE + DIB_HEADER_SIZE + NUM_PIXELS * BYTES_PER_PIXEL;
    bmp.pixel_array_offset = BMP_HEADER_SIZE + DIB_HEADER_SIZE;
    dib.size = 40;
    dib.width = xsize;
    dib.height = ysize;
    dib.color_planes = 1;
    dib.bits_per_pixel = 24;
    dib.compression_method = 0;
    dib.image_size = NUM_PIXELS * BYTES_PER_PIXEL;
    dib.hor_res = 0;
    dib.vert_res = 0;
    dib.num_of_colors = 0;
    dib.num_important_colors = 0;
    
    fwrite(&bmp,sizeof(char),BMP_HEADER_SIZE,file_out);
    fwrite(&dib,sizeof(char),DIB_HEADER_SIZE,file_out);
    
    pixel pixel_array[xsize][ysize];
    
    for (int x = 0; x < SIZE; ++x)
    {
        for (int y = 0; y < SIZE; ++y)
        {
            pixel_array[x][y].blue = image[x][y] % 3 * 40;
            pixel_array[x][y].red = image[x][y] % 5 * 30;
            pixel_array[x][y].green = 0;
        }
    }
    fwrite(*pixel_array,sizeof(char),NUM_PIXELS*3,file_out);
    fclose(file_out);

    return 0;
}