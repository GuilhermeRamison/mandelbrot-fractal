CXXFLAGS = -fopenmp

all: mandelbrot.cpp
	g++ mandelbrot.cpp -o mandel
	./mandel 100 3
	convert pixelmap.ppm fractal.png

openmp: mandelbrot.cpp
	g++ mandelbrot.cpp -o mandel $(CXXFLAGS) -lpthread
	./mandel 100 3
	convert pixelmap.ppm fractal.png
