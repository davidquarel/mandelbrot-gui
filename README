README
======

To compile, use the provided Makefile by executing `make`.
Run the program with `./mandelbrot` and pipe the result into a `.ppm` file.
The program prints a help message with `-h` or `--help` flags.

`usage: mandelbrot [-h | --help] [-d W H] [-f b|g|c] [-s S] [-i I] [-p X Y] [-r R]`

`-h | --help` Prints the help message

`-d W H` Specifies the size of the image, `W` is the width, `H` is the height.
Defaults to `-d 1024 1024`

`-f b|g|c` Selects the format to print in, defaults to `g` 

`b` is black and white in ASCII, which
can be viewed in the terminal and is mostly for debugging purposes. 

`g` is grey scale in binary,
where the shade of grey represents how fast a point escapes the Mandelbrot set, or black if it never
leaves the set.

`c` is color, where the program is ran again and again with different iterations depths, and the images
are all placed on top of each other. Much more expensive than `b` or `g`, would recommend running it
with a low value for iteration depth, around 30.

`-s S` is the difference in adjacent colours, defaults to 11.

`-i I` is the iteration depth when calculating the Mandelbrot set in B&W or grey scale, or the number of iterations to do
if in color. Default value is 255, recommended to lower to 30 when using color.

`-p X Y` is a coordinate point to centre the output picture around, default is `-p 0 0`.

`-r R` is the "radius", or half of the length of the size of the bounding box in which the picture is drawn, defaults to 2

The bottom left corner of the bounding box is (X-R,Y-R), and the top right is (X+R,Y+R)

Example: `-p 1 0 2` will draw the bounding box such that the bottom left corner is at (-1,-2), top right at (3,2)
	
		
		

