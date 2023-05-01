FLAGS = -Wall -fPIC
LIBFLAGS = -L./lib/binaries -I./lib/headers
LDFLAGS = -lGL -lglfw3
CFLAGS = -std=c99
CPPFLAGS = -std=c++0x
CC = g++

main: obj/main.o obj/mandelbrot.o obj/render.o
	$(CC) $(FLAGS) $(LIBFLAGS) -o bin/main obj/main.o obj/mandelbrot.o obj/render.o $(LDFLAGS)

obj/main.o: src/main.cpp
	$(CC) $(FLAGS) -c $(CPPFLAGS) src/main.cpp -o obj/main.o

obj/mandelbrot.o: src/generation/mandelbrot.c
	$(CC) $(FLAGS) -c $(CFLAGS) src/generation/mandelbrot.c -o obj/mandelbrot.o

obj/render.o: src/graphics/render.cpp
	$(CC) $(FLAGS) -c $(CPPFLAGS) src/graphics/render.cpp -o obj/render.o


clean:
	rm -f bin/main obj/main.o obj/mandelbrot.o obj/render.o
