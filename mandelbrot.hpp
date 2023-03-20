#ifndef MANDELBROT_CPP
#define MANDELBROT_CPP
#include <math.h>
#include <stdio.h>

const int resX = 1000;
const int resY = 1000;
const int iterations = 1000;
const int totalPoints = resX * resY;
// Things to do with this:
// Get OpenGL working and rendering points
// CUDA Multithreading
// Heterogeneous Processing
/**
 * @brief A representation of a point on the complex plane.
 */
struct Point {
    double r; // Real component
    double i; // Imaginary component
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
int checkMandelbrot(const Point& p) {
    int i = 0;
    Point lastP = { 0, 0 };

    while (i < iterations && lastP.r * lastP.r + lastP.i * lastP.i <= 4) {
        // squares the real and the imaginary components and adds them with the first iteration.
        double tempr = lastP.r;
        lastP.r = (lastP.r * lastP.r - lastP.i * lastP.i) + p.r;
        lastP.i = 2 * tempr * lastP.i + p.i;
        i++;
    }
    return i;
}

//void normalizeiterations(double* normaliter, int* iter, int* pixeliterations) {
//    int sumpixeliterations = 0;
//    for (int i = 0; i < iterations; i++) {
//        sumpixeliterations += pixeliterations[i];
//    }
//
//    for (int i = 0; i < resx; i++) {
//        for (int k = 0; k < resy; k++) {
//            int curindex = i * resx + k;
//            double numiteratpoint = iter[curindex];
//            iter[curindex] = 0.0;
//            for (int j = 0; j < numiteratpoint; j++) {
//                iter[curindex] += ((double)pixeliterations[j]) / sumpixeliterations;
//            }
//        }
//    }
//}

/**
 * @brief Generates a file of iterations from the mandelbrot set.
 *
 * @param Point topLeft, the top left point of the screen to render.
 * @param Point bottomRight, the bottom right point of the screen to render.
 * @param int numIterations, the maximum number of iterations of the mandelbrot equation to iterate through before terminating.
 * @param int resolutionX, the length of the screen in the horizontal plane.
 * @param int resolutionY, the length of the screen in the vertical plane.
 */
void genMandelbrot(double* vertices, double* colors, const Point& topLeft, const Point& bottomRight) {
    printf("Generating set...\n");
    double lenX = topLeft.r - bottomRight.r, lenY = topLeft.i - bottomRight.i;
    int pixelIterations[iterations] = { 0 };
    int* iter = new int[totalPoints];
    for (int i = 0; i < resX; i++) {
        for (int k = 0; k < resY; k++) {
            Point p = Point{ topLeft.r - (double) i / resX * lenX, topLeft.i - (double) k / resY * lenY };
            // Checks and outputs the number of iterations before the point diverges or terminates.
            int pIter = checkMandelbrot(p);
            iter[i*resX+k] = pIter;
            pixelIterations[pIter]++;
            // Gets normalized vertices for rendering.
            vertices[(i * resX + k) * 2] = ((double)(i * 2 - resX)) / (resX);
            vertices[(i * resX + k) * 2 + 1] = ((double)(k * 2 - resY)) / (resY);
        }
    }
    // Coloring in the set.
    printf("Coloring set...\n");
    for (int i = 0; i < totalPoints; i++) {
        if (iter[i] == iterations) {
            colors[i * 3] = 0.0;
            colors[i * 3 + 1] = 0.0;
            colors[i * 3 + 2] = 0.0;
        }
        else {
            double val = fmod(pow(((double)iter[i] / iterations) * 50, 1.5), 1);
            colors[i * 3] = val;
            colors[i * 3 + 1] = val;
            colors[i * 3 + 2] = 1.0 - val;
        }
    }
    delete[] iter;
}
#endif