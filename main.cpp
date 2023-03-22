#include "mandelbrot.cu"
#include "render.hpp"

int main() {
    Point tl = { -2, 1.25 }, br = { 0.5, -1.25 }, zm = { -0.748749753783, -0.069712607615 };
    double zoomFactor = 0.25;
    Bounds bds = { tl, br, br.r - tl.r, tl.i - br.i };
    initializeRenderer();
    while (true) {
        printf("Starting a new generation...\n");
        float* vertices = new float[totalPoints * 2];
        float* colors = new float[totalPoints * 3];
        genMandelbrot(vertices, colors, bds);
        bufferData(vertices, colors);
        render();
        printf("Finished rendering, moving on...\n");
        bds.tl.r += zoomFactor * (zm.r - tl.r);
        bds.tl.i -= zoomFactor * (tl.i - zm.i);
        bds.br.r -= zoomFactor * (br.r - zm.r);
        bds.br.i += zoomFactor * (zm.i - br.i);
        bds.lenX = br.r - tl.r;
        bds.lenY = br.i - tl.i;
    }
}