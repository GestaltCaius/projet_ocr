#define _XOPEN_SOURCE 700
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <SDL/SDL.h>
#include "pixel_operations.h"
#include "loadimage.h"

#include "neural.h"
#include "weight_file.h"


struct try
    *init_try_folder(char *path) {
        FILE *fp = fopen(path, "r");
        
        if(fp == NULL)
        {
            printf("wrong path");
            abort();
        }

        char *path_tmp = NULL;
        char i_char[50] ={0};
        ssize_t read;
        size_t n = 0;
        read = getline(&path_tmp,&n,fp);

        size_t nb_line = atoi(path_tmp) -1 ;
        printf("%zu", nb_line);
        size_t nb_char = 26 * 2 + 10;
        struct try
            *trys = calloc(nb_line * nb_char, sizeof(struct try));
        init_sdl();
        for(size_t i = 0; i < nb_line; i++)
        {
            read = getline(&path_tmp,&n,fp);
            if(read)
            {
                path_tmp[read-1] = '\0';
                for(size_t j = 0; j < nb_char; j++)
                {
                    i_char[0]='\0';
                    sprintf(i_char, "%zu", j);

                    char full_path[500] = {0};
                    strcpy(full_path, path_tmp);
                    strcat(full_path, i_char);
                    strcat(full_path, ".png");

                    SDL_Surface *img = load_image(full_path);
                    trys[i*nb_char + j].in = calloc(16 * 16, sizeof(double));
                    
                    for(int k = 0; k < 16; k++)
                    {
                        for(int l = 0; l < 16; l++)
                        {
                            trys[i*nb_char + j].in[k * (img->w) + l] = 1;
                        }
                    }

                    for(int k = 0; k < img->w; k++)
                    {
                        for(int l = 0; l < img->h; l++)
                        {
                            Uint32 pixel = getpixel(img,l,k);
                            Uint8 r = 0, g = 0, b = 0;
                            SDL_GetRGB(pixel, img->format, &r, &g, &b);
                            Uint8 res = r * 0.3 + g * 0.59 + b * 0.11;
                           if (res < 127)
                                trys[i*nb_char + j].in[k * (img->w) + l] = 1;
                            else    
                                trys[i*nb_char + j].in[k * (img->w) + l] = 0;
                        }
                    }
                    trys[i*nb_char+j].res = calloc(nb_char, sizeof(double)); 
                    trys[i*nb_char+j].res[j] = 1;
                }
            }
        }
    return trys;
    }

