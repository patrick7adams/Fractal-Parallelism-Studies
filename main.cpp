#include "mandelbrot.hpp"
#include "render.hpp"

int main() {
    Point tl = { -2, 1.25 }, br = { 0.5, -1.25 };
    int iter = 1000, resX = 1000, resY = 1000;
    render(iter, resX, resY, genMandelbrot(tl, br, iter, resX, resY));
}