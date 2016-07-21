#include "pixel_operations.h"
#include "filters.h"
#include "loadimage.h"
#include "segmentation.h"
#include "neural.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "weight_file.h"

static inline char num_to_char(int i)
{
    if(i < 10)
        return i+'0';

    if(i < 36)
        return 'A'+(i-10);

    return 'a'+(i-36);
}

char array_to_char(double *array) {
    int max = 0;
    for(int i = 0; i<10+26*2; i++)
    {
        if(array[i] > array[max])
            max = i;
    }

    if(max>0.6)
        return num_to_char(max);
    else
        return '!';
}

struct network tr_and_init_network() {
    size_t L[] = {16 * 16, 10, 10};
    struct network net = init_network(L, 3);
    struct try
        *tr = init_numbers_0_to_9("./nbs/");
    train(&net, tr, 10, 20000, 5000);
    free_trys(tr, 10);
    return net;
}

struct vector *create_vector(struct matrix *A) {
    struct vector *V = img_to_lines(A);
    struct vector *F = lines_to_char(A, V);
    F = resize_char(A, F);
    free_vector(V);
    return F;
}

void small_ocr(struct network *net, char *fname, struct matrix *A,
               struct vector *F) {
    FILE *file;
    if ((file = fopen(fname, "w")) == NULL) {
        err(3, "Error while creating %s", fname);
    }
    printf(" == BEGIN OCR == \n");
    for (size_t i = 0; i < F->size; i++) {
        if (F->data[i].w1 < 0) {
            if(F->data[i].w1 == -42)
            {
                printf(" ");
                fprintf(file, " ");
            }
            else
            {
                printf("\n");
                fprintf(file, "\n");
            }
        } else {
            double *in = resize_table(F->data[i], A, 16, 16);
            /*
                printf("\n");
            for(int i = 0; i< 16; i++)
            {
                for(int j = 0; j < 16; j++)
                {
                    printf("%d", (int)in[i*16+j]);
                }
                printf("\n");
            } 
                printf("\n");
                */
            feedforward(net, in);
            free(in);
            double *out = get_out(*net);
            char res = array_to_char(out);
            free(out);
            printf("%c", res);
            fprintf(file, "%c", res);
            fflush(stdout);
        }
    }
    fclose(file);
}

int main_ocr(int argc, char *filename, char *file_out) {
    if (argc == 2 || argc == 3) {
        init_sdl();
        SDL_Surface *img = load_image(filename);
        filter_greyscale(img);
        filter_blackwhite(img);
        display_image(img);
        struct matrix *A = img_to_matrix(img);

        struct vector *F = create_vector(A);

        display_segmentation(img, F);

        display_image(img);
        if (argc == 3) {
            struct network* net = load_network_from_file("ocr_weights.txt");

            small_ocr(net, file_out, A, F);

            printf("\n == END == \n");
            fflush(stdout);
            free_network_neurons(net);
        }
        free_vector(F);
        free_matrix(A);
        SDL_FreeSurface(img);
        SDL_Quit();

    } else {
        printf("Missing path");
    }
    return 0;
}
