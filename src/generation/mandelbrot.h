#pragma once

#include "../util/constants.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct Point {
    double r; // Real component
    double i; // Imaginary component
};

struct Bounds {
    struct Point tl;
    struct Point br;
    double lenX;
    double lenY;
};

void checkMandelbrot(int iStart, int iLen, int* iter, const struct Point* p);
void initializePointsAndVertices(float* vertices, struct Point *points, struct Bounds *bounds);
float* normalizeColors(int* iter);
void genColor(float* colors, int* iter);
void genMandelbrot(float* vertices, float* colors, struct Bounds *bounds);