#include "mandelbrot.h"
#include "mandel.h"
#include "mandel_cuda.cuh"

void initializeVertices(float* vertices) {
    printf("Initializing vertices...\n");
    for (int k = 0; k < resY; k++) {
        for (int i = 0; i < resX; i++) {
            vertices[(k * resX + i) * 2] = ((float)(i * 2.0f - resX)) / (resX);
            vertices[(k * resX + i) * 2 + 1] = ((float)(k * 2.0f - resY)) / (resY);
        }// x1, y1, x2, y2, x3, y3...
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

double genMandelbrot(float* vertices, float* colors, struct Bounds *bounds) {
    printf("Generating set...\n");
    
    int* iter = (int*) malloc(totalPoints * sizeof(int));
    struct Point* points = (struct Point*) malloc(sizeof(struct Point) * totalPoints);

    if (iter == NULL || points == NULL) {
        printf("iter or points did not allocate memory correctly");
        exit(EXIT_FAILURE);
    }

    initializePoints(points, bounds);
    double elapsedTime;
    char* threadTypeName;
    switch(threadingType) {
        case BASE:
            threadTypeName = "BASE";
            elapsedTime = base_gen(iter, points);
            break;
        case OMP:
            threadTypeName = "OMP";
            elapsedTime = omp_gen(iter, points);
            break;
        case CUDA:
            threadTypeName = "CUDA";
            elapsedTime = cuda_gen(iter, points);
            break;
    }
    printf("Utilizing %s threading\n", threadTypeName);
    printf("Time elapsed for this iteration: %f\n", elapsedTime);
    printf("Finished generating points!\n");
    genColor(colors, iter);
    
    free(iter);
    free(points);
    return elapsedTime;
}

