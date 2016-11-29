CC=gcc
 
CPPFLAGS= `pkg-config --cflags gtk+-2.0 --cflags sdl `
CFLAGS= -Wall -Wextra -std=c99 -O3 -g
LDFLAGS=
LDLIBS= `pkg-config --libs gtk+-2.0 sdl` -lSDL_image 
 
SRC= main.c image_operation.c
OBJ= ${SRC:.c=.o}
 
all: main
 
main: ${OBJ}
 
clean:
	rm -f *~ *.o
	rm -f main
 
