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
    printf("\x1B[32mStarting Initialization!\033[0m\n");
    Point tl = { -2, 1.25 }, br = { 0.5, -1.25 }, zm = { -0.748749753783, -0.069712607615 };
    Bounds bds = { tl, br, br.r - tl.r, tl.i - br.i };
    
    int iter = 0;
    float zoomFactor = 0.25f;

    GLFWwindow* window = initialize_window();

    float* colors = (float*)malloc(sizeof(float) * totalPoints * 3);
    float* vertices = (float*) malloc(totalPoints*2 * sizeof(float));
    initializeVertices(vertices);

    unsigned int shaderProgram = programInit();
    unsigned int vertex_buffer, color_buffer, VAO;

    initializeRenderer(&VAO, &vertex_buffer, &color_buffer, window);

    if (vertices == NULL || colors == NULL) {
            printf("Vertices or Colors did not allocate memory correctly");
            exit(EXIT_FAILURE);
    }
    printf("\x1B[32mFinished Initialization!\033[0m\n");

    while (iter < zoomDepth && !glfwWindowShouldClose(window)) {
        printf("\x1B[32mStarting generation %d!\033[0m\n", iter);
        printf("Bounds: (%f, %f) and (%f, %f)\n", bds.tl.r, bds.tl.i, bds.br.r, bds.br.i);
        
        genMandelbrot(vertices, colors, &bds);
        bufferData(vertices, colors, vertex_buffer, color_buffer);
        render(VAO, window, shaderProgram);
        // if (toggleSave) {
        //     save(colors, iter);
        // }
        zoom(&bds, &zm, zoomFactor);
        iter++;
    }
    free(vertices);
    free(colors);
    // compileVideo(iter);
    glfwTerminate();
}
