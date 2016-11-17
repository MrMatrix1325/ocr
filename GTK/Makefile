CC=gcc
 
CPPFLAGS= `pkg-config --cflags gtk+-2.0`
CFLAGS= -Wall -Wextra -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs gtk+-2.0`
 
SRC= test.c
OBJ= ${SRC:.c=.o}
 
all: test
 
test: ${OBJ}
 
clean:
	rm -f *~ *.o
	rm -f test
 
