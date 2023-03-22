#ifndef MANDELBROT_CPP
#define MANDELBROT_CPP
#include <math.h>
#include <stdio.h>
#include <cuda.h>
#include <cuda_runtime.h>

const int resX = 1000;
const int resY = 1000;
const int iterations = 1000;
const int totalPoints = resX * resY;
// Things to do with this:
// CUDA Multithreading
// Heterogeneous Processing
/**
 * @brief A representation of a point on the complex plane.
 */
struct Point {
    double r; // Real component
    double i; // Imaginary component
};

struct Bounds {
    Point tl;
    Point br;
    double lenX;
    double lenY;
};

/**
 * @brief Checks if a point is a part of the mandelbrot set.
 *
 * @param Point p, the point at which to check if it is a member of the mandelbrot set.
 * @param int numIterations, the maximum number of iterations of the mandelbrot equation to iterate through
 * before terminating.
 * @return int, the number of iterations that the point underwent before either the maximum number of
 * iterations is reached or the point diverges.
 */
__global__ void checkMandelbrot(int *iter, const Point& p) {
    Point lastP = { 0, 0 };
    while (*iter < iterations && lastP.r * lastP.r + lastP.i * lastP.i <= 4) {
        // squares the real and the imaginary components and adds them with the first iteration.
        double tempr = lastP.r;
        lastP.r = (lastP.r * lastP.r - lastP.i * lastP.i) + p.r;
        lastP.i = 2 * tempr * lastP.i + p.i;
        *iter++;
    }
}

__global__ void initializePointsAndVertices(float* vertices, Point *points, Bounds *bounds) {
    for (int i = 0; i < resX; i++) {
        for (int k = 0; k < resY; k++) {
            Point p = Point{ *bounds.tl.r - (double)i / resX * lenX, *tl.i - (double)k / resY * lenY };

            vertices[(i * resX + k) * 2] = ((float)(i * 2.0 - resX)) / (resX);
            vertices[(i * resX + k) * 2 + 1] = ((float)(k * 2.0 - resY)) / (resY);
        }
    }
}

/**
 * @brief Generates a file of iterations from the mandelbrot set.
 *
 * @param Point topLeft, the top left point of the screen to render.
 * @param Point bottomRight, the bottom right point of the screen to render.
 * @param int numIterations, the maximum number of iterations of the mandelbrot equation to iterate through before terminating.
 * @param int resolutionX, the length of the screen in the horizontal plane.
 * @param int resolutionY, the length of the screen in the vertical plane.
 */
void genMandelbrot(float* vertices, float* colors,  Bounds& bounds) {
    printf("Generating set...\n");
    
    int* iter = new int[totalPoints];
    Point* points = new Point[totalPoints];

    initializePointsAndVertices(vertices, points, bounds);
            int* pIter = 0;

            checkMandelbrot(pIter, p);

            iter[i*resX+k] = pIter;
            // Gets normalized vertices for rendering.
            
    // Coloring in the set.
    printf("Coloring set...\n");
    for (int i = 0; i < totalPoints; i++) {
        if (iter[i] == iterations) {
            colors[i * 3] = 0.0;
            colors[i * 3 + 1] = 0.0;
            colors[i * 3 + 2] = 0.0;
        }
        else {
            float val = fmod(pow(((float)iter[i] / iterations) * 50, 1.5), 1);
            colors[i * 3] = val;
            colors[i * 3 + 1] = val;
            colors[i * 3 + 2] = 1.0 - val;
        }
    }
    delete[] iter;
}
#endif