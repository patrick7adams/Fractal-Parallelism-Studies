#ifndef MANDELBROT_HPP
#define MANDELBROT_HPP
#include <iostream>
#include <fstream>
#include <math.h>
// Things to do with this:
// Get OpenGL working and rendering points
// CUDA Multithreading
// Heterogeneous Processing
/**
 * @brief A representation of a point on the complex plane.
 */
struct Point {
    float r; // Real component
    float i; // Imaginary component
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
int checkMandelbrot(const Point& p, const int numIterations) {
    int i = 0;
    Point lastP = { 0, 0 };

    while (i < numIterations && lastP.r * lastP.r + lastP.i * lastP.i <= 4) {
        // squares the real and the imaginary components and adds them with the first iteration.
        float tempr = lastP.r;
        lastP.r = (lastP.r * lastP.r - lastP.i * lastP.i) + p.r;
        lastP.i = 2 * tempr * lastP.i + p.i;
        i++;
    }
    return i;
}

/**
 * @brief Generates a file of iterations from the mandelbrot set.
 *
 * @param Point topLeft, the top left point of the screen to render.
 * @param Point bottomRight, the bottom right point of the screen to render.
 * @param int numIterations, the maximum number of iterations of the mandelbrot equation to iterate through
 * before terminating.
 * @param int resolutionX, the length of the screen in the horizontal plane.
 * @param int resolutionY, the length of the screen in the vertical plane.
 */
void genMandelbrot(const Point& topLeft, const Point& bottomRight, const int numIterations, const int resolutionX, const int resolutionY) {
    // creates output file
    std::ofstream output;
    output.open("output.txt");

    float lenX = topLeft.r - bottomRight.r, lenY = topLeft.i - bottomRight.i;
    for (float i = 0; i <= resolutionX; i++) {
        for (float k = 0; k <= resolutionY; k++) {
            // Finds the exact values to check the mandelbrot set with.
            float xVal = topLeft.r - i / resolutionX * lenX, yVal = topLeft.i - k / resolutionY * lenY;

            // Checks and outputs the number of iterations before the point diverges or terminates.
            int n = checkMandelbrot(Point{ xVal, yVal }, numIterations);
            output << n << " ";
            // output << "(" << xVal << ", " << yVal << ") ";
        }
        // creates a newline after each row
        output << "\n";
    }

    output.close();
}

#endif