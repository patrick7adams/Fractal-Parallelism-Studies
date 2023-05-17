INCLUDES = -I./lib/headers -I./src/util
FLAGS = -Wall -fPIC -g $(INCLUDES)
LIBFLAGS = -L./lib/binaries
LDFLAGS = -lGL -lglfw3
CFLAGS = -std=c99
CPPFLAGS = -std=c++0x
CC = g++

.PHONY: clean

main: obj/main.o obj/mandelbrot.o obj/render.o obj/glad.o
	$(CC) $(FLAGS) $(LIBFLAGS) -o $@ $^ $(LDFLAGS)

obj/main.o: src/main.cpp
	$(CC) $(FLAGS) $(INCLUDES) -o $@ -c $(CPPFLAGS) src/main.cpp

obj/mandelbrot.o: src/generation/mandelbrot.c
	$(CC) $(FLAGS) $(INCLUDES) -c $(CFLAGS) src/generation/mandelbrot.c -o obj/mandelbrot.o

obj/render.o: lib/headers/shader_constants.h src/graphics/render.cpp
	$(CC) $(FLAGS) $(INCLUDES) -c $(CPPFLAGS) src/graphics/render.cpp -o obj/render.o

obj/glad.o: lib/headers/shader_constants.h lib/headers/glad.c
	$(CC) $(FLAGS) -c $(CFLAGS) $^  -o $@

lib/headers/shader_constants.h: src/graphics/shaders/vertex.glsl src/graphics/shaders/fragment.glsl
	bash -c "xxd -i src/graphics/shaders/vertex.glsl | tac | sed '3s/$$/, 0x00/' | tac > lib/headers/shader_constants.h"
	bash -c "xxd -i src/graphics/shaders/fragment.glsl | tac | sed '3s/$$/, 0x00/' | tac >> lib/headers/shader_constants.h"

clean:
	rm -f bin/main obj/main.o obj/mandelbrot.o obj/render.o obj/glad.o obj/main lib/headers/shader_constants.h
