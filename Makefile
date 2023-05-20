INCLUDES = -I./lib/headers -I./src/util -I/opt/cuda/targets/x86_64-linux/include
FLAGS = -Wall -fPIC -g $(INCLUDES)
LIBFLAGS = -L./lib/binaries -L/opt/cuda/targets/x86_64-linux/lib -L/opt/cuda/targets/x86_64-linux/lib/stubs
LDFLAGS = -lGL -lglfw3 -lcuda -lcudart
CFLAGS = -std=c99
OPENMPFLAGS = -fopenmp
CPPFLAGS = -std=c++0x
CUDAFLAGS = 
CC = g++
CUDA = nvcc
OTHER  = lib/headers/shader_constants.h
OBJ = obj/glad.o obj/mandel_base.o obj/mandel_cuda.o obj/mandel_omp.o obj/mandelbrot.o obj/render.o obj/main.o


.PHONY: clean

all: main clean

main: $(OTHER) $(OBJ)
	$(CC) $(FLAGS) $(LIBFLAGS) -o $@ $^ $(LDFLAGS) $(OPENMPFLAGS)

obj/main.o: src/main.cpp
	$(CC) $(FLAGS) $(INCLUDES) -o $@ -c $(CPPFLAGS) src/main.cpp

obj/mandelbrot.o: src/generation/mandelbrot.c
	$(CC) $(FLAGS) $(INCLUDES) -c $(CFLAGS) $< -o $@

obj/mandel_base.o: src/generation/mandel_base.c
	$(CC) $(FLAGS) $(INCLUDES) -c $(CFLAGS) $< -o $@

obj/mandel_omp.o: src/generation/mandel_omp.c
	$(CC) $(FLAGS) $(INCLUDES) -c $(CFLAGS) $< -o $@ $(OPENMPFLAGS)

obj/mandel_cuda.o: src/generation/mandel_cuda.cu
	$(CUDA) $(INCLUDES) -c $(CUDAFLAGS) $< -o $@

obj/render.o: src/graphics/render.cpp lib/headers/shader_constants.h obj/glad.o
	$(CC) $(FLAGS) $(INCLUDES) -c $(CPPFLAGS) $< -o $@

obj/glad.o: lib/headers/glad.c lib/headers/shader_constants.h
	$(CC) $(FLAGS) -c $(CFLAGS) $< -o $@

lib/headers/shader_constants.h: src/graphics/shaders/vertex.glsl src/graphics/shaders/fragment.glsl
	bash -c "xxd -i src/graphics/shaders/vertex.glsl | tac | sed '3s/$$/, 0x00/' | tac > lib/headers/shader_constants.h"
	bash -c "xxd -i src/graphics/shaders/fragment.glsl | tac | sed '3s/$$/, 0x00/' | tac >> lib/headers/shader_constants.h"

clean:
	rm -f $(OBJ) $(OTHER)
