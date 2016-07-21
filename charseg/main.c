#include "pixel_operations.h"
#include "filters.h"
#include "loadimage.h"
#include "segmentation.h"
#include "neural.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

char array_to_char(int *array) {
    if (array[0] == 1)
        return '0';
    if (array[1] == 1)
        return '1';
    if (array[2] == 1)
        return '2';
    if (array[3] == 1)
        return '3';
    if (array[4] == 1)
        return '4';
    if (array[5] == 1)
        return '5';
    if (array[6] == 1)
        return '6';
    if (array[7] == 1)
        return '7';
    if (array[8] == 1)
        return '8';
    if (array[9] == 1)
        return '9';

    return 'e';
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
    struct vector *B = img_to_blocks(A);
    printf("\n      Number of blocks : %d \n", (int)B->size);
    //struct vector *Test = vector_make(1);
    //struct coords full_img;
    //full_img.w1 = 1, full_img.h1 = 1;
    //full_img.w2 = A->width - 1, full_img.h2 = A->height - 1;
    //vector_push_front(Test, full_img);
    struct vector *V = img_to_lines(A, B/*Test*/);
    printf("\n      Number of lines : %d \n\n", (int)V->size);
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
            printf("\n");
            fprintf(file, "\n");
        } else {
            double *in = resize_table(F->data[i], A, 16, 16);
            feedforward(net, in);
            free(in);
            int *out = get_bin_out(*net);
            char res = array_to_char(out);
            free(out);
            printf("%c", res);
            fprintf(file, "%c", res);
            fflush(stdout);
        }
    }
    fclose(file);
}

int main(int argc, char *argv[]) {
    // Test de contraste, gommage etc.
    if (argc == 3 && *argv[1] == 'r')
    {
        init_sdl();
        SDL_Surface *img = load_image(argv[2]);
        struct matrix *M = filter_greyscale_matrix(img);
        filter_noise(M);
        display_image(img);
        filter_contrast(M);
        matrix_to_img(M, img);
        display_image(img);
        SDL_FreeSurface(img);
        free_matrix(M);
        SDL_Quit();
    }


    if(argc == 3 && *argv[1] == 'l')
    {
        init_sdl();
        SDL_Surface *img = load_image(argv[2]);
        struct matrix *M = filter_greyscale_matrix(img);
        display_image(img);
for(int i = 0; i <= 1; i++)
        {
            filter_noise(M);
            //M = check_neighbourhood(M);
            matrix_to_img(M, img);
        }
        display_image(img);
        SDL_FreeSurface(img);
        SDL_Quit();
    }


    if (argc == 2 || argc == 3) {
        init_sdl();
        SDL_Surface *img = load_image(argv[1]);
        filter_greyscale(img);
        filter_blackwhite(img);
        display_image(img);
        struct matrix *A = img_to_matrix(img);

        struct vector *F = create_vector(A);

        display_segmentation(img, F);

        display_image(img);
        if (argc == 3) {
            struct network net = tr_and_init_network();

            small_ocr(&net, argv[2], A, F);

            printf("\n == END == \n");
            fflush(stdout);
            free_network_neurons(&net);
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
