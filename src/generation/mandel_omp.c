#include "mandel.h"
#include <omp.h>
#include <stdio.h>

void omp_check_points(int ID, int num_threads, int* iter, const struct Point* p) {
    double tempr;
    int it;
    for (int i = ID; i < totalPoints; i+=num_threads) {
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

double omp_gen(int* iter, const struct Point* p) {
    double t = omp_get_wtime();
    int max_threads = omp_get_max_threads();
    #pragma omp parallel
    {
        int ID = omp_get_thread_num();
        omp_check_points(ID, max_threads, iter, p);
    }
    t = omp_get_wtime()-t;
    return t;
}