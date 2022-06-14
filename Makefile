FLAGS = -fopenmp -lpthread

all: mandelbrot.cpp
	mpiCC mandelbrot.cpp -o mandel
	./mandel 100 3
	convert pixelmap.ppm fractal.png

openmp: mandelbrot.cpp
	mpiCC mandelbrot.cpp -o mandel $(FLAGS)
	./mandel 100 3
	convert pixelmap.ppm fractal.png

omp-mpi: mandelbrot.cpp
	mpiCC mandelbrot.cpp -o mandel $(FLAGS)
	mpirun -np 2 --hostfile hosts ./mandel 100 3
	convert pixelmap.ppm fractal.png