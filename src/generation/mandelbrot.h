#pragma once

#include "../util/constants.h"

#include <string.h>
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

void initializePoints(struct Point *points, struct Bounds *bounds);
void initializeVertices(float* vertices);

void checkMandelbrot(int iStart, int iLen, int* iter, const struct Point* p);
float* normalizeColors(int* iter);
void genColor(float* colors, int* iter);
void genMandelbrot(float* vertices, float* colors, struct Bounds *bounds);
