#include "mandelbrot.hpp"
#include "render.hpp"

#define resX 1000
#define resY 1000
#define iterations 1000

int main() {
    Point tl = { -2, 2 }, br = { 2, -2 };
    render(genMandelbrot(tl, br, iterations, resX, resY));
}