#ifndef __CONSTANTS__
#define __CONSTANTS__

struct Point {
    double r; // Real component
    double i; // Imaginary component
};

struct Bounds {
    struct Point tl;
    struct Point br;
    double lenX;
    double lenY;
};

const int leftBound = -2;
const int rightBound = -1.5;
const int topBound = 1.25;
const int bottomBound = -1.25;

const int resX = 1000;
const int resY = 1000;
const int maxIterations = 1000;
const int totalPoints = resX * resY;

// const int useMandelbrot = 1;
// const int juliaInputZX = 0;
// const int juliaInputZY = 0;
// const int circularJulia = 0;

// const int toggleSave = 0;
// const int disableRendering = 0;
// const int FPS = 60;

const int zoomDepth = 100;
// const int useZoomControls = 0;

enum RunType {
        BASE = 1,
        OMP = 2,
        CUDA = 3
};
const int threadingType = CUDA;


#endif