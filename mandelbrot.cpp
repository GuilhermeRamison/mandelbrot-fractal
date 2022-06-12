#include <iostream>
#include <fstream>
#include <complex>
#include <chrono>
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
    

    if (my_Image.is_open ()) {
        my_Image << "P3\n" << WIDTH << " " << HEIGHT << " 255\n";

        auto tStart = std::chrono::high_resolution_clock::now();
        //MPI...
        #pragma parallel openmp for
        for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT; y++)  {
                int val = value(x, y, atoi(argv[1]), atoi(argv[2]));
                my_Image << val << ' ' << 0 << ' ' << 0 << "\n";
            }
        }
        auto tEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> fTime = tEnd - tStart;
        cout << "Time Taken: " + std::to_string(fTime.count()) << "s" << "\n";

        my_Image.close();
    }
    else
      cout << "Could not open the file";

    return 0;
}