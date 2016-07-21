# Makefile

#CC=clang -fsanitize=address
CC=gcc -g #-fsanitize=address
CPPFLAGS= -MMD -MP `pkg-config --cflags sdl`
CFLAGS= -Wall -Wextra -pedantic -std=c99 -O2
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lm

SRC= train.c pixel_operations.c loadimage.c neural.c weight_file.c
OBJ= ${SRC:.c=.o}


all: neural

neural: ${OBJ}

.PHONY: clean

clean:
	rm -f neural neural.d
	rm -f *.o *.d
