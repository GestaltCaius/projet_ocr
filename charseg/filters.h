#ifndef FILTERS_H
#define FILTERS_H
#include <stdio.h>
#include <SDL/SDL_image.h>
#include "pixel_operations.h"
#include "segmentation.h"

// Greyscale
void filter_greyscale(SDL_Surface *img);

// Black and White
void filter_blackwhite(SDL_Surface *img);

// Convolution

void filter_noise(struct matrix *M);

void filter_contrast(struct matrix *M);

void filter_flou(struct matrix *M);

void filter_repoussage(struct matrix *M);

void convolution_apply(struct matrix *M, double *C);

/*void convolution_apply(struct matrix *M, int w, int h, struct matrix *conv);

void filter_noise(struct matrix *M);

void filter_contrast(struct matrix *M);*/

// img to/from matrix

struct matrix *filter_greyscale_matrix(SDL_Surface *img);

void matrix_to_img(struct matrix *M, SDL_Surface *img);

#endif
