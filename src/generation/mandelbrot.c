#include "mandelbrot.h"

void initializeVertices(float* vertices) {
    printf("Initializing vertices...\n");
    for (int k = 0; k < resY; k++) {
        for (int i = 0; i < resX; i++) {
            vertices[(k * resX + i) * 2] = ((float)(i * 2.0f - resX)) / (resX);
            vertices[(k * resX + i) * 2 + 1] = ((float)(k * 2.0f - resY)) / (resY);
        }
    }
    printf("Initialized vertices!\n");
}

void initializePoints(struct Point *points, struct Bounds *bounds) {
    printf("Initializing points and vertices...\n");
    for (int k = 0; k < resY; k++) {
        for (int i = 0; i < resX; i++) {
            points[k*resX+i] = (struct Point){ bounds->tl.r + (double)i / resX * bounds->lenX, bounds->tl.i - (double)k / resY * bounds->lenY };
            
        }
    }
    printf("Initialized points from (%f, %f) to (%f, %f)\n", points[0].r, points[0].i, points[totalPoints - 1].r, points[totalPoints - 1].i);
}

void checkMandelbrot(int iStart, int iLen, int* iter, const struct Point* p) {
    double tempr;
    int it;
    for (int i = iStart; i < iStart + iLen; i++) {
        struct Point lastP = { 0.0, 0.0 };
        it = 0;
        while (it < maxIterations && lastP.r * lastP.r + lastP.i * lastP.i <= 4) {
            // squares the real and the imaginary components and adds them with the first iteration.
            tempr = lastP.r;
            lastP.r = (lastP.r * lastP.r - lastP.i * lastP.i) + p[i].r;
            lastP.i = 2.0 * tempr * lastP.i + p[i].i;
            it++;
        }
        iter[i] = it;
        // printf("Iteration %d at Point (%f, %f): %d\n", i, p[i].r, p[i].i, iter[i]);
    }
}

float* normalizeColors(int* iter) {
    float* hue = (float*)calloc(totalPoints, sizeof(float));
    if (hue == NULL) {
        printf("Hue is null after malloc!");
        exit(EXIT_FAILURE);
    }
    int numIterationsPerPixel[maxIterations] = {0};

    int pixelSum = 0;
    for (int i = 0; i < totalPoints; i++) {
        numIterationsPerPixel[iter[i]]++;
    }
    for (int i = 0; i < maxIterations; i++) {
        pixelSum += numIterationsPerPixel[i];
    }
    for (int i = 0; i < totalPoints; i++) {
        for (int k = 0; k < iter[i]; k++) {
            hue[i] += ((float) numIterationsPerPixel[k]) / pixelSum;
        }
    }
    return hue;
}

void genColor(float* colors, int* iter) {
    printf("Coloring set...\n");

    // looks nice, but very computationally intensive; use iter instead of this for now
    // float* hue = normalizeColors(iter);
    for (int i = 0; i < totalPoints; i++) {
        if (iter[i] == maxIterations) {
            colors[i * 3] = 0.0f;
            colors[i * 3 + 1] = 0.0f;
            colors[i * 3 + 2] = 0.0f;
        }
        else {
            float val = fmod(pow(pow((float)iter[i]/maxIterations, 1.5f)*50.0f, 1.5f), 1.0f);
            // printf("%f ", val);
            colors[i * 3] = val;
            colors[i * 3 + 1] = 1.0-val;
            colors[i * 3 + 2] = val;
        }
    }
    // free(hue);
}

void genMandelbrot(float* vertices, float* colors, struct Bounds *bounds) {
    printf("Generating set...\n");
    
    int* iter = (int*) malloc(totalPoints * sizeof(int));
    struct Point* points = (struct Point*) malloc(sizeof(struct Point) * totalPoints);

    if (iter == NULL || points == NULL) {
        printf("iter or points did not allocate memory correctly");
        exit(EXIT_FAILURE);
    }

    initializePoints(points, bounds);

    int lenSection = 1000;
    int numSections = totalPoints / lenSection;
    for (int i = 0; i < numSections; i++) {
        checkMandelbrot(i * lenSection, lenSection, iter, points);
    }

    printf("Finished generating points!\n");
    // checkMandelbrot(499000, 1000, iter, points);
    // Gets normalized vertices for rendering.
            
    // Coloring in the set.
    genColor(colors, iter);
    
    free(iter);
    free(points);
}

