#include "filters.h"

// Greyscale
void filter_greyscale(SDL_Surface *img) {
    Uint32 pxl;
    Uint8 r, g, b;
    for (int w = 0; w < img->w; w++) {
        for (int h = 0; h < img->h; h++) {
            pxl = getpixel(img, w, h);
            SDL_GetRGB(pxl, img->format, &r, &g, &b);
            r = 0.3 * r + 0.59 * g + 0.11 * b;
            pxl = SDL_MapRGB(img->format, r, r, r);
            putpixel(img, w, h, pxl);
        }
    }
}


// Black and White
void filter_blackwhite(SDL_Surface *img) {
    Uint32 pxl;
    Uint8 r;
    for (int w = 0; w < img->w; w++) {
        for (int h = 0; h < img->h; h++) {
            pxl = getpixel(img, w, h);
            SDL_GetRGB(pxl, img->format, &r, &r, &r);
            r = r > 255 / 2 ? 255 : 0;
            pxl = SDL_MapRGB(img->format, r, r, r);
            putpixel(img, w, h, pxl);
        }
    }
}


// Noise reduction using convolution techniques

void filter_noise(struct matrix *M)
{
    double *C = malloc(sizeof(double) * 9); //the convolution matrix (3x3).
    for(int i = 0; i < 9; i++)
        C[i] = 1;
    C[4] = 5;
    convolution_apply(M, C);
    free(C);
}

void filter_contrast(struct matrix *M)
{
    double *C = calloc(9, sizeof(double));
    C[1] = C[3] = C[5] = C[7] = -1, C[4] = 5;
    convolution_apply(M, C);
    free(C);
}

void filter_flou(struct matrix *M)
{
    double *C = malloc(sizeof(double) * 9);
    for(int i = 0; i < 9; i++)
        C[i] = 1;
    convolution_apply(M, C);
    free(C);
}

void filter_repoussage(struct matrix *M)
{
    double *C = malloc(sizeof(double) * 9);
    C[0] = -2, C[1] = C[3] = -1, C[4] = C[5] = C[7] = 1, C[6] = C[2] = 0, C[8] = 2;
    convolution_apply(M, C);
    free(C);
}

void convolution_apply(struct matrix *M, double *C)
{
    int coef = 0; //the summ of the coefficients in the convolution matrix.
    for(int i = 0; i < 9; i++)
        coef += C[i];

    double *Out = malloc(M->width * M->height * sizeof(double));
    /*for(size_t i = 0; i < M->width; i++)
    {
        Out[i] = 255;
        Out[M->width * M->height - 1] = 255;
    }
    for(size_t i = 1; i < M->height; i++)
    {
        Out[i * M->height] = 255;
        Out[i * M->height + M->width -1] = 255;
    }*/
    for(size_t i = 0; i < M->width * M->height; i++)
    {
        Out[i] = 255;
    }

    //The main function :
    for(size_t h = 1; h < M->height - 1; h++)
    {
        for(size_t w = 1; w < M->width - 1; w++)
        {
            double summ = 0;
            int pos = 0;
            for(int i = -1; i < 2; i++)
            {
                for(int j = -1; j < 2; j++)
                {
                    //int pos = (i + 1) * 3 + (j + 1);
                    summ += M->data[(h + i) * M->width + (w + j)] * C[pos];
                    pos++;
                }
            }
            if(summ < 0)
                summ = 0;
            else
                summ = (double)((int)(summ / coef) % 256);
            Out[h * M->width + w] = summ;
        }
    }

    //We whiten the borders :
    /*
    size_t margin = (int)M->height / 20;
    printf("\nVertical margin : %d \n", (int)margin);
    for(size_t i = 0; i < margin; i++)
    {
        for(size_t j = 0; j < M->width; j++)
        {
            Out[i * M->width + j] = 255;
            Out[(M->height - i - 1) * M->width + j] = 255;
        }
    }

    margin = (int)M->width / 20;
    printf("\nHorizontal margin : %d \n", (int)margin);
    for(size_t i = 0; i < M->height; i++)
    {
        for(size_t j = 0; j < margin; j++)
        {
            Out[i * M->width + j] = 255;
            Out[i * M->width + M->width - j - 1] = 255;
        }
    }
    */
    free(M->data);
    M->data = Out;
}

/*void filter_noise(struct matrix *M)
{
  // Create conv matrix
  struct matrix *conv = malloc(sizeof(struct matrix));
  conv->width = conv->height = 3;
  conv->data = malloc(conv->width *conv->height * sizeof(double));
  for(size_t i = 0; i < conv->width; i++)
      for(size_t j = 0; j<conv->height; j++)
          conv->data[i * conv->height + j] = (i == (conv->width/2) && j == (conv->height/2))
              ? 5 : 1; // all 1's, except for the 5 in the center
  // Let's apply the conv matrix
  for(size_t i = 1; i < M->width - 1; i++)
  {
    for(size_t j = 1; j < M->height - 1; j++)
      convolution_apply(M, i, j, conv);
  }
}

// Subfunction that apply the convulation matrix to one elt of our grey matrix 
// DO NOT call it on the edges, or it'll be out of range
void convolution_apply(struct matrix *M, int w, int h, struct matrix *conv)
{
  double sum = 0;
  for(int i = 0; w - 1 + i <= w + 1; i++)
  {
    for(int j = 0; h - 1 + j < h + 1; j++)
      sum += M->data[ (w-1+i) 
          + M->width * (h-1+j)] * conv->data[i * 3 + j];
  }
  M->data[w + M->width * h] = sum / 9;
}


// Contrast filter

void filter_contrast(struct matrix *M)
{
  // Create convolution matrix
  struct matrix *conv = malloc(sizeof(struct matrix));
  conv->width = conv->height = 3;
  conv->data = malloc(9 * sizeof(double));
  for(int i = 0; i < 9; ++i)
  {
    if (i % 2)
      conv->data[i] = 1;
    else
      conv->data[i] = i == 4 ? 5 : 0;
  }
  // Apply filter
  for(size_t i = 1; i < M->width - 1; ++i)
  {
    for(size_t j = 1; j < M->height - 1; ++j)
    {
      convolution_apply(M, i, j, conv);
    }
  }
}*/

// Matrix operations
struct matrix *filter_greyscale_matrix(SDL_Surface *img) {
    Uint32 pxl;
    Uint8 r, g, b;
    struct matrix *M = malloc(sizeof(struct matrix));
    M->data = malloc(img->w * img->h * sizeof(double));
    M->width = img->w;
    M->height = img->h;
    for (int w = 0; w < img->w; w++) {
        for (int h = 0; h < img->h; h++) {
            pxl = getpixel(img, w, h);
            SDL_GetRGB(pxl, img->format, &r, &g, &b);
            r = 0.3 * r + 0.59 * g + 0.11 * b;
            M->data[w + img->w *  h] = (double)r; 
        }
    }
    return M;
}

/* we use the img gave in filter_greyscale_matrix
 * that way, its elements are already filled (format, height, width) */
void matrix_to_img(struct matrix *M, SDL_Surface *img)
{
  Uint32 pxl;
  Uint8 rgb;
  for (int w = 0; w < img->w; w++)
  {
    for (int h = 0; h < img->h; h++)
    {
      rgb = (Uint8)M->data[w + M->width * h];
      pxl = SDL_MapRGB(img->format, rgb, rgb, rgb);
      putpixel(img, w, h, pxl);
    }
  }
}
