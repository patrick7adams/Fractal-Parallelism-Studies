#ifndef MANDEL_H
#define MANDEL_H
#include "constants.h"
#include <time.h>

double base_gen(int* iter, const struct Point* p);
double omp_gen(int* iter, const struct Point* p);

void base_check_points(int iStart, int iLen, int* iter, const struct Point* p);
void omp_check_points(int ID, int num_threads, int* iter, const struct Point* p);

#endif