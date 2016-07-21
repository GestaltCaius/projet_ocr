# GTK Makefile
# $ gcc -o simple simple.c `pkg-config --libs --cflags gtk+-2.0`
 
# Compilers and options
CC=gcc -fsanitize=address
CPPFLAGS=
CFLAGS= -Wall -Wextra -std=c99 -O2 `pkg-config --cflags gtk+-2.0`
LDFLAGS= 
LDLIBS= `pkg-config --libs sdl gtk+-2.0` -lSDL_image -lm

SRC= ${wildcard *.c}
OBJ= ${SRC:.c=.o}
 
all: main
 
main: ${OBJ}
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)
	
clean:
	rm -f *~ *.o
	rm -f main

# END

