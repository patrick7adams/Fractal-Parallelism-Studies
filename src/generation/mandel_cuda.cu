#include "mandel_cuda.cuh"

__global__ void cuda_check_points(int iLen, int* iter, struct Point* p) {
    int iStart = blockIdx.x * blockDim.x + threadIdx.x;
    double tempr;
    int it;
    if(iStart*iLen < totalPoints) {
        for (int i = iStart*iLen; i < iStart*iLen + iLen; i++) {
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
}

double cuda_gen(int* iter, const struct Point* p) {
    int* cuda_iter;
    cudaMalloc(&cuda_iter, sizeof(int) * totalPoints);
    struct Point* cuda_p;
    cudaMalloc(&cuda_p, sizeof(struct Point)*totalPoints);
    cudaMemcpy(cuda_p, p, sizeof(struct Point)*totalPoints, cudaMemcpyHostToDevice);

    cudaEvent_t start,end;
    float timeElapsed;

    int numThreads = 128;
    int numBlocks = 256;
    int iLen = totalPoints / (numThreads*numBlocks)+1;

    cudaEventCreate(&start);
    cudaEventCreate(&end);
    cudaEventRecord(start, 0);
    cuda_check_points<<<numThreads, numBlocks>>>(iLen, cuda_iter, cuda_p);
    cudaEventRecord(end, 0);
    cudaEventSynchronize(end);

    cudaMemcpy(iter, cuda_iter, sizeof(int)*totalPoints, cudaMemcpyDeviceToHost);
    cudaFree(cuda_iter);
    cudaFree(cuda_p);

    cudaEventElapsedTime(&timeElapsed, start, end);
    return ((double)timeElapsed)/1000.0; // convert ms to s
}