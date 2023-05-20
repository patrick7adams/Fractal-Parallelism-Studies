#include "mandel.h"
#include <stdio.h>

void base_check_points(int iStart, int iLen, int* iter, const struct Point* p) {
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

double base_gen(int* iter, const struct Point* p) {
    clock_t t = clock();
    base_check_points(0, totalPoints, iter, p);
    t = clock() - t;
    double diff = (double)t/CLOCKS_PER_SEC;
    return diff;
}