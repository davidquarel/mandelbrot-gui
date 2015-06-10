README
======
UPDATE: Now supports the Julia set (only in colour)

To compile, use the provided Makefile by executing `make`.
Run the program with `./mandelbrot` and pipe the result into a `.ppm` file.
The program prints a help message with `-h` or `--help` flags.

`usage: mandelbrot [-h | -help] [-window WIDTH HEIGHT] 
			[-mandel] [-julia X Y] [-bw | -grey | -color | -color-step STEP]
			[-iter I] [-origin X Y] [-radius R] > file.ppm`

`-h | --help` Prints the help message

`-window WIDTH HEIGHT` Specifies the size of the image, defaults to `1024x1024`

`-mandel` Selects the mandelbrot fractal

`-julia X Y` Selects the Julia Set with C = X + Yi

All output is in `.ppm` format.

`-bw` Output in black and white ASCII `(P1)`, which
can be viewed in the terminal and is mostly for debugging purposes. 

`-grey` Output is grey scale in binary `(P5)`
where the shade of grey represents how fast a point escapes.

`-color` Output is color, where the program is ran again and again with different iterations depths, and the images
are all placed on top of each other. Much more expensive than `-bw` or `-grey`, would recommend running it
with a low value for iteration depth, around 30.

`-color-step STEP` Same as above, but can manually set colour step (the difference in colour between each iteration
step, default is 11.)

`-iter I` is the iteration depth when calculating the fractal in B&W or grey scale, or the number of iterations to do
if in color. Default value is 100, recommended to lower to 30 when using color.

`-origin X Y` The coordinate point to centre the output picture around, default is `-origin 0 0`.

`-radius R` is the "radius", or half of the length of the size of the bounding box in which the picture is drawn, defaults to 2

The bottom left corner of the bounding box is (X-R,Y-R), and the top right is (X+R,Y+R)

Example: `-origin 1 0 -radius 2` will draw the bounding box such that the bottom left corner is at (-1,-2), top right at (3,2).
	

<!--
A point $c$ is in the Mandelbrot set if the sequence $(z_n)$ defined by
$$
z_{n+1} = z_n^2 + c
$$
is unbounded. The entire Mandelbrot set is contained within a ball of radius 2 centred at the origin, so
it is sufficient to check if any point in the sequence ever leaves this ball. In practise only finitely many iterations
are done, and if after these iterations the point still has not left the set, we assume it never will. A greater iteration depth
results in greater accuracy, and an better approximation to the true Mandelbrot set.

We also cheat and check that
$$
\forall n, \; |z_n|^2 < 4 \text{ rather than } \forall n \; |z_n| < 2
$$
as the unnecessary square root is expensive.
-->
		
		

