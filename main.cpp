#include "mandelbrot.c"
#include "render.hpp"

// constants (get from GUI later)
// bounds
#define leftBound = -2
#define rightBound = 0.5
#define topBound = 1.25
#define bottomBound = -1.25

// resolution
#define resolutionX 1000
#define resolutionY 1000

// Color settings

// ADD HERE LATER

// Equation settings

// 1 to use mandelbrot, 0 to use julia.
#define useMandelbrot 1
#define juliaInputZX 0
#define juliaInputZY 0
// Preset.
#define circularJulia 0

// Save settings
#define toggleSave 1
#define disableRendering 0
#define frameFrequency 60

// Zoom settings
// #define zoomFactor 0.25
#define zoomDepth 20
#define useZoomControls 0








// misc
#define maxIterations 1000

void zoom(Bounds* bds, Point* zm, float zoomFactor) {
    bds->tl.r += zoomFactor * (zm->r - bds->tl.r);
    bds->tl.i -= zoomFactor * (bds->tl.i - zm->i);
    bds->br.r -= zoomFactor * (bds->br.r - zm->r);
    bds->br.i += zoomFactor * (zm->i - bds->br.i);
    bds->lenX = bds->br.r - bds->tl.r;
    bds->lenY = bds->tl.i - bds->br.i;
}

void compileVideo(int iter) {
    // do something here
}

int main() {
    Point tl = { -2, 1.25 }, br = { 0.5, -1.25 }, zm = { -0.748749753783, -0.069712607615 };
    Bounds bds = { tl, br, br.r - tl.r, tl.i - br.i };
    initializeRenderer();
    int iter = 0;
    float zoomFactor = 0.1f;
    float* vertices = (float*)malloc(sizeof(float) * totalPoints * 2);
    float* colors = (float*)malloc(sizeof(float) * totalPoints * 3);
    while (iter < zoomDepth) {
        printf("Starting generation %d!\n", iter);
        printf("Bounds: (%f, %f) and (%f, %f)\n", bds.tl.r, bds.tl.i, bds.br.r, bds.br.i);
        if (vertices == NULL || colors == NULL) {
            printf("Vertices or Colors did not allocate memory correctly");
            exit(EXIT_FAILURE);
        }
        genMandelbrot(vertices, colors, &bds);
        bufferData(vertices, colors);
        render();
        if (toggleSave) {
            save(colors, iter);
        }
        zoom(&bds, &zm, zoomFactor);
        iter++;
    }
    free(vertices);
    free(colors);
    compileVideo(iter);
}