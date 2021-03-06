## IPPD-UFPEL course work

This code aims to draw the mandelbrot fractal set using MPI nodes to perform calculations.

Written in c++.
Ref for the mandelbrot calculation: https://github.com/SlachPi/mandelbrot_cpp

### Dependencies

> Debian
>
>> ` sudo apt install build-essential mpi mpich libomp-dev imagemagick`
>>
>
> Fedora
>
>> `sudo dnf groupinstall @development-tools @development-libraries mpi mpich imagemagick`
>>

### Build & Run

- make mpi
  - Build and run the application without using openMP. May be slow for higher iterarions on the calculation
- make omp-mpi
  - Build and run the application using openMP

![fractal-image](https://github.com/GuilhermeRamison/mandelbrot-fractal/blob/main/fractal.png?raw=true)
