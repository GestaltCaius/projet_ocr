#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <SDL.h>
#include "pixel_operations.h"
#include "loadimage.h"

#include "neural.h"
#include "train.h"
#include "weight_file.h"


// inits fonctions
struct neuron init_neuron(size_t nw, int ent) {
    struct neuron new;
    new.nw = nw;
    new.w = calloc(nw, sizeof(size_t));
    new.lw = calloc(nw, sizeof(size_t));

    // set random weight;
    if (ent) {
        for (size_t i = 0; i < nw; i++) {
            new.w[i] = 0;
        }
        new.b = 0;
    } else {
        for (size_t i = 0; i < nw; i++) {
            new.w[i] = (((double)rand() / (double)RAND_MAX) - 0.5) * 4;
        }

        // set random bias
        new.b = (((double)rand() / (double)RAND_MAX) - 0.5) * 4;
    }
    // set last vals to 0
    new.lder = 0;
    new.lout = 0;

    return new;
}

void free_network_neurons(struct network *net) {
    for (size_t i = 0; i < net->nL; i++) {
        for (size_t j = 0; j < net->L[i]; j++)
        {
            free(net->n[i][j].w);
            free(net->n[i][j].lw);
        }
        free(net->n[i]);
    }
    free(net->n);
}

struct network init_network(size_t *L, size_t nL) {
    struct network new;
    new.L = L;
    new.nL = nL;

    // create neurons
    new.n = calloc(nL, sizeof(struct neuron *));
    for (size_t i = 0; i < nL; i++) {
        new.n[i] = calloc(L[i], sizeof(struct neuron));
        for (size_t j = 0; j < L[i]; j++) {
            if (i == 0) {
                new.n[i][j] = init_neuron(0, 1);
            } else {
                new.n[i][j] = init_neuron(L[i - 1], 0);
            }
        }
    }
    return new;
}

struct try
    *init_try_xor() {
        struct try
            *trys = calloc(4, sizeof(struct try));

        double input[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
        double output[4] = {0, 1, 1, 0};

        for (int i = 0; i < 4; i++) {
            (trys[i]).in = calloc(2, sizeof(double));
            (trys[i]).in[0] = input[i][0];
            (trys[i]).in[1] = input[i][1];

            (trys[i]).res = calloc(1, sizeof(double));
            (trys[i]).res[0] = output[i];
        }

        return trys;
    }

struct try
    *init_try_3_2() {
        struct try
            *trys = calloc(8, sizeof(struct try));
        double input[8][3] = {{0, 0, 0},
                              {0, 0, 1},
                              {0, 1, 0},
                              {0, 1, 1},
                              {1, 0, 0},
                              {1, 0, 1},
                              {1, 1, 0},
                              {1, 1, 1}};
        double output[8][2] = {
            {0, 0}, {0, 1}, {1, 0}, {1, 1}, {1, 1}, {1, 0}, {0, 1}, {0, 0}};

        for (int i = 0; i < 8; i++) {
            (trys[i]).in = calloc(3, sizeof(double));
            (trys[i]).in[0] = input[i][0];
            (trys[i]).in[1] = input[i][1];
            (trys[i]).in[2] = input[i][2];

            (trys[i]).res = calloc(2, sizeof(double));
            (trys[i]).res[0] = output[i][0];
            (trys[i]).res[1] = output[i][1];
        }
        return trys;
    }

struct try
    *init_numbers_0_to_9(char *path) {
        struct try
            *trys = calloc(10, sizeof(struct try));
        init_sdl();
        char str[80], i_char[80];
        for (int i = 0; i < 10; i++) {
            // create path to file
            strcpy(str, path);
            sprintf(i_char, "%d", i);
            strcat(str, i_char);
            strcat(str, ".png");
            // printf(str);
            // printf("\n");

            // load image
            SDL_Surface *img = load_image(str);
            trys[i].in = calloc(img->w * img->h, sizeof(double));

            // fill array
            printf("%d \n", img->w * img->h);
            for (int j = 0; j < img->w; j++) {
                for (int k = 0; k < img->h; k++) {
                    Uint32 pixel = getpixel(img, k, j);
                    Uint8 r = 0, g = 0, b = 0;
                    SDL_GetRGB(pixel, img->format, &r, &g, &b);
                    Uint8 res = r * 0.3 + g * 0.59 + b * 0.11;
                    if (res < 127)
                        trys[i].in[j * (img->w) + k] = 1;
                    else
                        trys[i].in[j * (img->w) + k] = 0;

                    printf("%d ", (int)(trys[i].in[j * (img->w) + k]));
                }
                printf("\n");
            }
            (trys[i]).res = calloc(10, sizeof(double));
            (trys[i]).res[i] = 1;
            SDL_FreeSurface(img);
        }
        SDL_Quit();
        return trys;
    }

void free_trys(struct try *tries, size_t number) {
    for (size_t i = 0; i < number; i++) {
        free(tries[i].in);
        free(tries[i].res);
    }
    free(tries);
}

double *get_out(struct network net) {
    double *out = calloc(net.L[net.nL - 1], sizeof(double));
    for (size_t i = 0; i < net.L[net.nL - 1]; i++) {
        out[i] = net.n[net.nL - 1][i].lout;
    }
    return out;
}

int *get_bin_out(struct network net) {
    int *out = calloc(net.L[net.nL - 1], sizeof(double));
    for (size_t i = 0; i < net.L[net.nL - 1]; i++) {
        if (net.n[net.nL - 1][i].lout > 0.1)
            out[i] = 1;
        else
            out[i] = 0;
    }
    return out;
}

// sigmoid

double sigmoid(double z) { return 1.0 / (1.0 + exp(-z)); }

// dérivée sigmoid
double psigmoid(double z) {
    double sig = sigmoid(z);
    return sig * (1.0 - sig);
}

double dif_error(double y, double ti) { return 0.5 * (y - ti) * (y - ti); }
double dif_cross_error(double y, double ti) { return -ti*log(y)-(1-ti)*log(1-y); }

double dot_product(struct neuron *a, double *b, size_t n) {
    double result = 0;
    for (size_t i = 0; i < n; i++)
        result += a[i].lout * b[i];
    return result;
}

void feedforward(struct network *n, double *in) {
    double zj;
    for (size_t i = 0; i < (*n).nL; i++) {
        for (size_t j = 0; j < (*n).L[i]; j++) {
            if (i == 0) {
                (*n).n[i][j].lout = in[j];
            } else {
                // zf = x[] . w[] + b
                zj = dot_product((*n).n[i - 1], (*n).n[i][j].w, (*n).L[i - 1]) +
                     (*n).n[i][j].b;
                (*n).n[i][j].lin = zj;
                (*n).n[i][j].lout = sigmoid(zj);
            }
        }
    }
}

void backpropa(struct network *n, double eta, struct try in) {
    double der = 0;
    for (size_t i = (*n).nL - 1; i > 0; i--) {
        for (size_t j = 0; j < (*n).L[i]; j++) {
            if (i == (*n).nL - 1) // out layer
            {
                // w_k -= eta * (out_(i)(j) - res_voulu) * sig'((out_(i-1)[] .
                // w_i[]) + b_i) * out_(i-1)(k)
                (*n).n[i][j].lder = ((*n).n[i][j].lout - in.res[j]);
                (*n).n[i][j].lder *= psigmoid(
                    dot_product((*n).n[i - 1], (*n).n[i][j].w, (*n).L[i - 1]) +
                    (*n).n[i][j].b);
                for (size_t k = 0; k < (*n).L[i - 1]; k++) {

                    der = (*n).n[i][j].lder * (*n).n[i - 1][k].lout;
                    (*n).n[i][j].w[k] -= eta * der;
                }
                // bias
                //   printf("\n der1: %f\n",der);
                (*n).n[i][j].b -= eta * (*n).n[i][j].lder;
            } else // hiden layer
            {
                // w_k -= eta * sum( lder_(i+1)(j) * w_(i+1)(j) ) *
                // sig'((out_(i-1)[] . w_i[]) + b_i) * out_(i - 1)(k)
                (*n).n[i][j].lder = 0;

                for (size_t l = 0; l < (*n).L[i + 1]; l++) {
                    (*n).n[i][j].lder +=
                        (*n).n[i + 1][l].lder * (*n).n[i + 1][l].w[j];
                }
                (*n).n[i][j].lder *= psigmoid(
                    dot_product((*n).n[i - 1], (*n).n[i][j].w, (*n).L[i - 1]) +
                    (*n).n[i][j].b);
                for (size_t k = 0; k < (*n).L[i - 1]; k++) {
                    (*n).n[i][j].w[k] -=
                        eta * (*n).n[i][j].lder * (*n).n[i - 1][k].lout;
                }
                (*n).n[i][j].b -= eta * (*n).n[i][j].lder;
            }
        }
    }
}

static inline void swap_weights(double** a, double** b)
{
    double* c = *a;
    *a = *b;
    *b = c;
}

void backpropagation(struct network *n, double k, struct try in)
{
    //Output layer
    int l = n->nL-1;
    for(size_t i = 0; i < n->L[l]; i++)
    {
        n->n[l][i].lder = n->n[l][i].lout - in.res[i];
        //Weight adjustement

        
        /*free(n->n[l][i].lw);
        n->n[l][i].lw = malloc(sizeof(double) * n->L[l-1]);
        memcpy(n->n[l][i].lw, n->n[l][i].w, sizeof(double) * n->L[l-1]);
        */

        swap_weights(&(n->n[l][i].lw), &(n->n[l][i].w));
        

        for(size_t j = 0; j < n->L[l-1]; j++)
        {
            n->n[l][i].w[j] =n->n[l][i].lw[j] - k * n->n[l-1][j].lout * n->n[l][i].lder;
        }
        n->n[l][i].b -= k * n->n[l][i].lder;
    }

    //Hidden layers
    for(l = n->nL - 2; l > 0; l--)
        for(size_t i = 0; i < n->L[l]; i++)
        {
            /*
            free(n->n[l][i].lw);
            n->n[l][i].lw = malloc(sizeof(double) * n->L[l-1]);
            memcpy(n->n[l][i].lw, n->n[l][i].w, sizeof(double) * n->L[l-1]);
            */
            swap_weights(&(n->n[l][i].lw), &(n->n[l][i].w));


            n->n[l][i].lder = 0;
            for(size_t j = 0; j < n->L[l+1]; j++)
                n->n[l][i].lder += n->n[l+1][j].lder * n->n[l+1][j].lw[i];

            n->n[l][i].lder /= n->L[l+1];
            n->n[l][i].lder *= psigmoid(n->n[l][i].lin);

            for(size_t w = 0; w < n->n[l][i].nw; w++)
                n->n[l][i].w[w] = n->n[l][i].lw[w] -  k* n->n[l][i].lder
                    //* n->n[l][i].lout
                    * n->n[l-1][w].lout;

            n->n[l][i].b -= k * n->n[l][i].lder;
        }
}

// random sort array
void shuffle_ex(struct try *array, size_t n) {
    if (n > 1) {
        size_t i;
        for (i = 0; i < n; i++) {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            struct try
                t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

void train(struct network *net, struct try *tr, size_t nbval, size_t nite,
           size_t display) {
    double *result;
    for (size_t i = 0; i <= nite; i++) {
        shuffle_ex(tr, nbval);
        if (display > 0 && i % display == 0)
            printf("%zu:\n", i);
        double total_tryS = 0;
        for (size_t j = 0; j < nbval; j++) {
            
            static double in[16*16] = {0};
            memcpy(in, tr[j].in, 16*16*sizeof(double));
            int nonce = rand()%5;
            if( nonce == 0)
            {
                for(int y = 15; y > 0; y--)
                {
                    for(int x = 0; x < 16; x++)
                        in[y*16+x] = tr[j].in[(y-1)*16 + x];
                }   
                for(int x = 0; x < 16; x++)
                    in[x]=0;
            }
            else if(nonce == 1)
            {
                for(int x = 15; x > 0; x--)
                {
                    for(int y = 0; y < 16; y++)
                        in[y*16+x] = tr[j].in[y*16 + x -1];
                }   
                for(int y = 0; y < 16; y++)
                    in[y*16]=0;
            }
            else if(nonce == 2)
            {
                for(int y = 0; y < 15; y++)
                {
                    for(int x = 0; x < 16; x++)
                        in[y*16+x] = tr[j].in[(y+1)*16 + x];
                }   
                for(int x = 0; x < 16; x++)
                    in[x+15*16]=0;
            }
            else if(nonce == 3)
            {
                for(int x = 0; x < 15; x++)
                {
                    for(int y = 0; y < 16; y++)
                        in[y*16+x] = tr[j].in[y*16 + x+1];
                }   
                for(int y = 0; y < 16; y++)
                    in[y*16+15]=0;
            }

            feedforward(net, in);
            if (display > 0 && i % display == 0) {
                result = get_out(*net);

                if(nonce != 4)
                    printf("DECALAGE! %d\n", nonce);

                printf("\n\tIN:\n\t");
                fflush(stdout);
                for (size_t k = 0; k < (*net).L[0]; k++)
                {
                    if(k%16 == 0)
                        printf("\n\t|");
                    //printf(" %d |", (int)tr[j].in[k]);
                    if((int)in[k])
                        printf("X");
                    else
                        printf(" ");
                }

                printf("\n\tOUT:\n\t|");
                for (size_t k = 0; k < (*net).L[(*net).nL - 1]; k++)
                    printf(" %f |", result[k]);

                printf("\n\tRES:\n\t|");
                for (size_t k = 0; k < (*net).L[(*net).nL - 1]; k++)
                    printf(" %f |", tr[j].res[k]);

                printf("\n\tERROR:\n\t|");
                double total_try = 0;
                for (size_t k = 0; k < (*net).L[(*net).nL - 1]; k++) {
                    double dif = dif_error(result[k], tr[j].res[k]);
                    printf(" %f |", dif);
                    total_try += dif;
                }
                total_try = total_try / (*net).L[(*net).nL - 1];
                printf("\n\tTOTAL ERROR: %f\n", total_try);
                total_tryS += total_try;
                printf("\n");
                free(result);
            }
            backpropagation(net, 0.1, tr[j]);
        }

        if (display > 0 && i % display == 0) {
            total_tryS = total_tryS / nbval;
            printf("\nTOTAL ERROR TRYS (%zu): %f\n", i, total_tryS);

            printf("\n\n");
        }
    }
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("error args");
        return 0;
    }

    srand(time(NULL));

    if (argv[1][0] == '1') // xor
    {
        size_t L[] = {2, 4, 1};
        struct network net = init_network(L, 3);
        struct try
            *tr = init_try_xor();
        train(&net, tr, 4, 20000, 5000);
        free_trys(tr, 4);
        save_network_to_file(&net, "xor_weights.txt");
        free_network_neurons(&net);
        return 0;
    }

    if (argv[1][0] == '2') // 3 input 2 out ex
    {
        size_t L[] = {3, 7, 2};
        struct network net = init_network(L, 3);
        struct try
            *tr = init_try_3_2();
        train(&net, tr, 8, 20000, 5000);
        free_trys(tr, 8);
        free_network_neurons(&net);
        return 0;
    }

    if (argv[1][0] == '3') {
        size_t L[] = {16 * 16, 10, 10};
        struct network net = init_network(L, 3);
        struct try
            *tr = init_numbers_0_to_9("./nbs/");
        train(&net, tr, 10, 20000, 5000);
        free_trys(tr, 10);
        free_network_neurons(&net);
        return 0;
    }

    if (argv[1][0] == '4') {
        struct network *net = load_network_from_file("xor_weights.txt");
        struct try
            *tr = init_try_xor();
        train(net, tr, 4, 1, 1);
        free_network_neurons(net);
        free_trys(tr, 4);
        free(net->L);
        free(net);
        return 0;
    }

    if(argv[1][0] == '5') {
        struct try* tr = init_try_folder("../ocr-bibl/training/files.txt");
        size_t L[] = {16*16,2000,NB_CHAR};
        struct network net = init_network(L, 3);
        train(&net, tr, /*todo:get this value from file*/ NB_POL*NB_CHAR, 150, 10);
        
        save_network_to_file(&net, "ocr_weights.txt");
    }

    if(argv[1][0] == '6') {
        struct try* tr = init_try_folder("/home/khoudl_y/ocr/ocr-bibl/training/files.txt");
    }
    printf("error args");
    printf("end");
    fflush(stdout);

    return 0;
}
