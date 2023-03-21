#include "mandelbrot.hpp"
#include "render.hpp"

int main() {
    Point tl = { -2, 1.25 }, br = { 0.5, -1.25 }, zm = { -0.748749753783, -0.069712607615 };
    double zoomFactor = 0.25;
    initializeRenderer();
    while (true) {
        printf("Starting a new generation...\n");
        float* vertices = new float[totalPoints * 2];
        float* colors = new float[totalPoints * 3];
        genMandelbrot(vertices, colors, tl, br);
        bufferData(vertices, colors);
        render();
        printf("Finished rendering, moving on...\n");
        tl.r += zoomFactor * (zm.r - tl.r);
        tl.i -= zoomFactor * (tl.i - zm.i);
        br.r -= zoomFactor * (br.r - zm.r);
        br.i += zoomFactor * (zm.i - br.i);
    }
}