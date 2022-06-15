FLAGS = -fopenmp -lpthread

mpi: mandelbrot.cpp
	mpiCC mandelbrot.cpp -o mandel
	mpirun -np 2 --hostfile hosts ./mandel 64 3
	convert pixelmap.ppm fractal.png

omp-mpi: mandelbrot.cpp
	mpiCC mandelbrot.cpp -o mandel $(FLAGS)
	mpirun -np 2 --hostfile hosts ./mandel 100 3
	convert pixelmap.ppm fractal.png