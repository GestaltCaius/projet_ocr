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
