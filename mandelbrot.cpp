#include <iostream>
#include <fstream>
#include <complex>
#include <chrono>
#include <omp.h>
#include <mpi.h>

#define WIDTH 800
#define HEIGHT 800

using namespace std;

int value (int x, int y, int iter, float scale)  {
    
    float y_const = scale*0.75;
    float x_const = scale*0.5;

    complex<float> point(scale*(float)y/WIDTH-y_const, scale*(float)x/HEIGHT-x_const);
    complex<float> z(0, 0);
    int nb_iter = 0;

    while (abs (z) < 2 && nb_iter <= iter) {
        z = z * z + point;
        nb_iter++;
    }
    
    if (nb_iter < iter)
       return (255*nb_iter)/iter;
    else
       return 0;
}

int main (int argc, char** argv)  {

    ofstream my_Image ("pixelmap.ppm");
    
    int rank, num_ranks, err;
    MPI_Status status;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    auto tStart = std::chrono::high_resolution_clock::now();

    /* 
        vamos atribuir o nó 0 como sendo o principal onde envia uma matriz vazia
        para outro nó e recebe a matriz calculada, e depois disso printa para o arquivo ppm

    */
    if (rank == 0) { 

        int img[WIDTH][HEIGHT];

        err = MPI_Send(img, WIDTH*HEIGHT, MPI_INT, 1, 0, MPI_COMM_WORLD);
        if (err != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, err);

        cout<< "Matriz enviada do nó 0 para o nó 1" <<" nó:" << rank <<"\n";
        
        err = MPI_Recv(img, WIDTH*HEIGHT, MPI_INT, 1, 0, MPI_COMM_WORLD,
            &status);
        if (err != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, err);
        cout<< "Matriz recebida do nó 1 para o nó 0" <<" nó:" << rank <<"\n";

        
        if (my_Image.is_open ()) {

            my_Image << "P3\n" << WIDTH << " " << HEIGHT << " 255\n";

            for (int x = 0; x < WIDTH; x++) {
                for (int y = 0; y < HEIGHT; y++)  {
                    my_Image << img[x][y] << ' ' << 0 << ' ' << 0 << "\n";
                }
            }
            
            my_Image.close();
        }
        else
            cout << "Could not open the file";        
    }
    else {

        int img2[WIDTH][HEIGHT]; //outra instancia de matriz para provar que está ocorrendo a transferencia via MPI

        err = MPI_Recv(img2, WIDTH*HEIGHT, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        if (err != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, err);

        cout<< "Matriz recebida do nó 0 para o nó 1" <<" nó:" << rank <<"\n";;
        
        #pragma omp parallel for
        for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT; y++)  {
                int val = value(x, y, atoi(argv[1]), atoi(argv[2]));
                img2[x][y] = val;
            }
        }
        cout<< "Mandelbrot calculado\n";
        
        err = MPI_Send(img2, WIDTH*HEIGHT, MPI_INT, 0, 0, MPI_COMM_WORLD);
        if (err != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, err);

        cout<< "Matriz calculada enviada do nó 1 para o nó 0" <<" nó:" << rank <<"\n";;
    }

    auto tEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> fTime = tEnd - tStart;
    cout << "Time Taken: " + std::to_string(fTime.count()) << "s" << "\n";

    MPI_Finalize();
    return 0;
}