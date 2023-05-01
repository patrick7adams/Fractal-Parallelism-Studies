#include "generation/mandelbrot.h"
#include "graphics/render.hpp"

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
    return;
}

int main() {
    Point tl = { -2, 1.25 }, br = { 0.5, -1.25 }, zm = { -0.748749753783, -0.069712607615 };
    Bounds bds = { tl, br, br.r - tl.r, tl.i - br.i };
    
    int iter = 0;
    float zoomFactor = 0.1f;
    float* vertices = (float*)malloc(sizeof(float) * totalPoints * 2);
    float* colors = (float*)malloc(sizeof(float) * totalPoints * 3);
    unsigned int shaderProgram, vertex_buffer, color_buffer;
    unsigned int VAO;
    GLFWwindow* window;

    initializeRenderer(&shaderProgram, &VAO, &vertex_buffer, &color_buffer, window);

    if (vertices == NULL || colors == NULL) {
            printf("Vertices or Colors did not allocate memory correctly");
            exit(EXIT_FAILURE);
    }

    while (iter < zoomDepth) {
        printf("Starting generation %d!\n", iter);
        printf("Bounds: (%f, %f) and (%f, %f)\n", bds.tl.r, bds.tl.i, bds.br.r, bds.br.i);
        
        genMandelbrot(vertices, colors, &bds);
        bufferData(vertices, colors, vertex_buffer, color_buffer);
        render(VAO, window, shaderProgram);
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
