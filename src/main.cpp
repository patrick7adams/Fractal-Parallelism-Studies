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
    Point tl = { -2, 1.25 }, br = { 0.5, -1.25 }, 
    zm = { -0.748749753783, -0.069712607615 }; // cool 1
    // zm = { -0.104893985, -0.927865454 }; // minibrot cool
    // zm = { -0.748478657, -0.069193702 }; // cool 2
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
    double times[zoomDepth] = {0.0};

    printf("\x1B[32mFinished Initialization!\033[0m\n");

    while (iter < zoomDepth && !glfwWindowShouldClose(window)) {
        printf("\x1B[32mStarting generation %d!\033[0m\n", iter);
        printf("Bounds: (%f, %f) and (%f, %f)\n", bds.tl.r, bds.tl.i, bds.br.r, bds.br.i);
        
        times[iter] = genMandelbrot(vertices, colors, &bds);
        bufferData(vertices, colors, vertex_buffer, color_buffer);
        render(VAO, window, shaderProgram);
        zoom(&bds, &zm, zoomFactor);
        iter++;
    }
    printf("\x1B[32mFinished iterating, exiting program!\033[0m\n");
    free(vertices);
    free(colors);
    // compileVideo(iter);
    glfwTerminate();
    printf("All times recorded in %d iterations:\n(", iter);
    for(int i = 0; i < iter-1; i++) {
        printf("%f, ", times[i]);
    }
    printf("%f)\n", times[iter-1]);
    double averageTime = 0.0;
    for(int i = 0; i < iter; i++) {
        averageTime+=times[i];
    }
    printf("Average time elapsed: %f\n", averageTime/iter);
}
