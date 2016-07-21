# Basic Makefile
 
# Compilers and options
CC=gcc #-fsanitize=address
CPPFLAGS=
CFLAGS= -Wall -Wextra -std=c99 -O2
LDFLAGS= 
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lm

SRC= ${wildcard *.c}
OBJ= ${SRC:.c=.o}
 
all: main
 
main: ${OBJ}
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)
	
clean:
	rm -f *~ *.o
	rm -f main

# END

