#include <stdio.h>
#include <stdlib.h>

#include "weight_file.h"

int save_network_to_file(struct network *net, char *filename) {
    FILE *f;

    if (!(f = fopen(filename, "w")))
        return -1;

    fprintf(f, "%zu\n", net->nL);

    for (size_t l = 0; l < net->nL; l++)
        fprintf(f, "%zu ", net->L[l]);

    fprintf(f, "\n");

    // Write the weight (then bias) of the output layer, then of the hidden one
    for (size_t l = net->nL - 1; l > 0; l--) {
        for (size_t n = 0; n < net->L[l]; n++) {
            for (size_t w = 0; w < net->n[l][n].nw; w++)
                fprintf(f, "%a ", net->n[l][n].w[w]);
            fprintf(f, "%a\n", net->n[l][n].b);
        }
    }

    fclose(f);

    return 0;
}

struct network *load_network_from_file(char *filename) {
    FILE *f;
    if (!(f = fopen(filename, "r")))
        return 0;

    printf("Reading sizes\n");
    fflush(stdout);

    struct network *net = malloc(sizeof(struct network));
    fscanf(f, "%zu\n", &net->nL);
    net->L = calloc(net->nL, sizeof(size_t));

    for (size_t l = 0; l < net->nL; l++)
        fscanf(f, "%zu ", net->L + l);

    fscanf(f, "\n");

    printf("Reading weights\n");
    fflush(stdout);

    net->n = calloc(net->nL, sizeof(struct neuron *));

    for (size_t l = net->nL - 1; l > 0; l--) {
        net->n[l] = calloc(net->L[l], sizeof(struct neuron));
        for (size_t n = 0; n < net->L[l]; n++) {
            net->n[l][n].nw = net->L[l - 1];
            net->n[l][n].w = calloc(net->n[l][n].nw, sizeof(double));
            net->n[l][n].lw = calloc(net->n[l][n].nw, sizeof(double));
            for (size_t w = 0; w < net->n[l][n].nw; w++)
                fscanf(f, "%la ", &net->n[l][n].w[w]);
            fscanf(f, "%la\n", &net->n[l][n].b);
        }
    }

    net->n[0] = calloc(net->L[0], sizeof(struct neuron));
    for (size_t n = 0; n < net->L[0]; n++) {
        net->n[0][n].nw = 0;
        net->n[0][n].w = NULL;
        net->n[0][n].lw = NULL;
        net->n[0][n].b = 0;
    }

    return net;
}
