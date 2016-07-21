#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "pixel_operations.h"
#include "loadimage.h"
#include "vector.h"

struct matrix {
    double *data;
    int width, height;
};

void test_charseg(SDL_Surface *originalimg, struct vector *v);

struct matrix *img_to_matrix(SDL_Surface *img);

void free_matrix(struct matrix *A);

struct vector *img_to_lines(struct matrix *img);

struct vector *lines_to_char(struct matrix *img, struct vector *lines);

struct vector *resize_char(struct matrix *img, struct vector *chars);

double *resize_table(struct coords c, struct matrix *A, int x, int y);

void draw_square(SDL_Surface *img, struct coords square);

void display_segmentation(SDL_Surface *img, struct vector *V);

/*
 *
 * struct coords { // coordinates of a pixel in the image (similar to a
Tuple<int, int> in c#)
        int h, w
};

struct list_coords { // List of pairs of coordinates
        stuct coords pos_1, pos_2;
        struct list_coords *next;
};

// BASIC FUNCTIONS

static inline
int list_is_empty(struct list_coords list) {return l -> next == NULL; }

struct list_coords* list_empty(void);

void print_coords(struct list_coords);

void list_coords* list_push_front(struct list_coords *list struct list_coords
*cell);

struct list_coords* list_pop_front(struct list_coords *list);

// CHARACTERS SEGMENTATION





        Sends back the heights of the beginning (pos_1) and of the end (pos_2)
of each line in the given image,
        which corresponds to the first line with pixels belonging to yet
unidentified characters, and the last
        pixel line before the end of those characters.

        To identify them and save them :
                - we browse the image from the top to the bottom.
                - when there is a big enough proportion (to determine) of black
pixels in a single line and close to
                  one another, we consider that we're at the beginning of a
line. we save the coordinates of the current
                        height in a struct coords (coords -> h = the height,
coords -> w is irrelevant).
                - We do the opposite than before : we browse every pixel line
under the recorded one until we find one
                  with proportionnally a lot (to determine) more white pixels
than black. We save	its location in another
                        struct coords.
                - We create a struct list_coords with in it the coordinates of
the two struct coords previously created
                  (pos_1 = beginning of the line, pos_2 = end of the line).
                - We find the coordinates of the next line under the one that
we've found by repeating the method described
                  above, we save them in another struct list_coords, and we link
the new record to the previous one
                        (line_n -> next = line_(n+1)).
                - When we reach the end of the image, we set the next line under
the last line found to NULL, so that
                  we know when we've reached the coordinates of the last
character line.

                        ABOUT THE PROPORTIONS :
                        - If we consider that a character cannot be composed of
a single black pixel (cf the dot of the 'i' in the shell),
                          then an easy way not to take into account potential
parasits could be to consider lonely pixels as irrelevant :
                          we do not take them into account.
                        - We could write a function that counts the number of
neighbours of every black pixel to determine its importance :
                          for example, if a pixel is part of a "stain" composed
by 14 pixels, and if the minimal number of pixels in
                                a potential character is fixed to 10, then the
pixel on which we are is eligible to be part of a character and
                                of a characters line ; if we have enough (to
determine) pixels like the previous one, then we're on a line.

struct list_coords* segmentation_to_lines (SDL_SURFACE *img); // Only the h
value of the struct coords matters in that one.

        Gives back the list of the coordinates of the corners on the top left
and bottom right corners of a square containing a
        character, for each character a given line.

struct list_coords* line_to characters(SDL_SURFACE *img, struct list_coords
*line);

        DEPENDING ON YOUNES AND THEO'S EXPECTATIONS.

        Uses segmentation_to_lines and list_coords to create an ordonate struct
list_coords ; the characters are put in it in the same
        order than they are on the picture, and marks the end of a line by
putting a struct list coords with pos_1 and pos_2 fixed on
        (-1,-1) after the coordinates of the last character of the line.

struct list_coords* segmentation_final_output(SDL_SURFACE *img, struct
list_coords *lines);

*/
