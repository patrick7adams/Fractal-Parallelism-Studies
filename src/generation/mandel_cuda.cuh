extern "C" {
    #include "constants.h"
    #include <stdio.h>
}
#include <cuda_runtime.h>

// #ifndef CONSTANTS_H
// const int resX = 1000;
// const int resY = 1000;
// const int maxIterations = 1000;
// const int totalPoints = resX * resY;

// struct Point {
//     double r; // Real component
//     double i; // Imaginary component
// };
// #endif

double cuda_gen(int* iter, const struct Point* p);
__global__ void cuda_check_points(int* iter, const struct Point* points);