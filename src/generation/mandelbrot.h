#ifndef MANDELBROT_H
#define MANDELBROT_H

#include "../util/constants.h"

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void initializePoints(struct Point *points, struct Bounds *bounds);
void initializeVertices(float* vertices);
float* normalizeColors(int* iter);
void genColor(float* colors, int* iter);
double genMandelbrot(float* vertices, float* colors, struct Bounds *bounds);
#endif
