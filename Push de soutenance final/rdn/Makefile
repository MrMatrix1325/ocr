CC=gcc
CPPFLAGS= `pkg-config --cflags sdl`
CFLAGS= -Wall -Wextra -Werror -pedantic -std=c99 -O3 -g
LDFLAGS= ${SRC}
LDLIBS= `pkg-config --libs sdl` -lSDL_image

SRC= rdn.c rdn_images.c
OBJ= ${SRC:.c=}

all:
 
main: ${OBJ}

clean:
	rm -f *~ *.o *.exe
 