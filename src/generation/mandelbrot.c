#include "mandelbrot.h"

/**
 * @brief Checks if a point is a part of the mandelbrot set.
 *
 * @param Point p, the point at which to check if it is a member of the mandelbrot set.
 * @param int numIterations, the maximum number of iterations of the mandelbrot equation to iterate through
 * before terminating.
 * @return int, the number of iterations that the point underwent before either the maximum number of
 * iterations is reached or the point diverges.
 */
void checkMandelbrot(int iStart, int iLen, int* iter, const struct Point* p) {
    for (int i = iStart; i < iStart + iLen; i++) {
        if (i < totalPoints) {
            struct Point lastP = { 0, 0 };
            while (iter[i] < maxIterations && lastP.r * lastP.r + lastP.i * lastP.i <= 4) {
                // squares the real and the imaginary components and adds them with the first iteration.
                double tempr = lastP.r;
                lastP.r = (lastP.r * lastP.r - lastP.i * lastP.i) + p[i].r;
                lastP.i = 2 * tempr * lastP.i + p[i].i;
                iter[i]++;
            }
        }
    }
}

void initializePointsAndVertices(float* vertices, struct Point *points, struct Bounds *bounds) {
    printf("Initializing points and vertices...\n");
    for (int k = 0; k < resY; k++) {
        for (int i = 0; i < resX; i++) {
            points[k*resX+i] = (struct Point){ bounds->tl.r + (double)i / resX * bounds->lenX, bounds->tl.i - (double)k / resY * bounds->lenY };
            vertices[(k * resX + i) * 2] = ((float)(i * 2.0 - resX)) / (resX);
            vertices[(k * resX + i) * 2 + 1] = ((float)(k * 2.0 - resY)) / (resY);
        }
    }
    printf("Initialized points from (%f, %f) to (%f, %f)\n", points[0].r, points[0].i, points[totalPoints - 1].r, points[totalPoints - 1].i);
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
float* normalizeColors(int* iter) {
    float* hue = (float*)malloc(sizeof(float) * totalPoints);
    if (hue == NULL) {
        printf("Hue is null after malloc!");
        exit(EXIT_FAILURE);
    }
    int numIterationsPerPixel[maxIterations];
    memset(numIterationsPerPixel, 0, maxIterations*sizeof(int));
    int pixelSum = 0;
    for (int i = 0; i < totalPoints; i++) {
        numIterationsPerPixel[iter[i]]++;
        hue[i] = 0.0f;
    }
    for (int i = 0; i < maxIterations; i++) {
        pixelSum += numIterationsPerPixel[i];
    }
    for (int i = 0; i < totalPoints; i++) {
        for (int k = 0; k < maxIterations; k++) {
            hue[i] += ((float) numIterationsPerPixel[k]) / pixelSum;
        }
    }
    return hue;
}

void genColor(float* colors, int* iter) {
    printf("Coloring set...\n");
    float* hue = normalizeColors(iter);
    for (int i = 0; i < totalPoints; i++) {
        if (iter[i] == maxIterations) {
            colors[i * 3] = 0.0;
            colors[i * 3 + 1] = 0.0;
            colors[i * 3 + 2] = 0.0;
        }
        else {
            float val = fmod(pow((hue[i] / maxIterations), 1.5f)*50, 50);
            colors[i * 3] = 1.0f - val;
            colors[i * 3 + 1] = 1.0f - val;
            colors[i * 3 + 2] = val;
        }
    }
}

void genMandelbrot(float* vertices, float* colors, struct Bounds *bounds) {
    printf("Generating set...\n");
    
    int* iter = (int*)malloc(sizeof(int) * totalPoints);
    struct Point* points = (struct Point*)malloc(sizeof(struct Point) * totalPoints);

    if (iter == NULL || points == NULL) {
        printf("iter or points did not allocate memory correctly");
        exit(EXIT_FAILURE);
    }

    initializePointsAndVertices(vertices, points, bounds);

    int lenSection = 1000;
    int numSections = totalPoints / lenSection;
    for (int i = 0; i < numSections; i++) {
        checkMandelbrot(i * lenSection, lenSection, iter, points);
    }
    // Gets normalized vertices for rendering.
            
    // Coloring in the set.
    genColor(colors, iter);
    
    free(iter);
    free(points);
}

