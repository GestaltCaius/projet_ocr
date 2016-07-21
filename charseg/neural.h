#ifndef NEURAL_H_INCLUED
#define NEURAL_H_INCLUED

struct neuron {
    double *w;   // tableau de poids
    size_t nw;   // nbs de poids
    double b;    // bias
    double lder; // dernière valeur des dériver (error_der * sig_der)
    double lout; // dernière sortie
};

struct network {
    size_t *L;         // tailles des layers
    size_t nL;         // nombre de layers
    struct neuron **n; // tableau de neurone (L[i] * nl);
};

struct try {
    double *in;  // essai entrée
    double *res; // essai resultat attendu
};

struct neuron init_neuron(size_t nw, int ent);

void free_network_neurons(struct network *net);

struct network init_network(size_t *L, size_t nL);

struct try
    *init_numbers_0_to_9(char *path);

void free_trys(struct try *tries, size_t number);

double *get_out(struct network net);

int *get_bin_out(struct network net);

void feedforward(struct network *n, double *in);

void train(struct network *net, struct try *tr, size_t nbval, size_t nite,
           size_t display);

#endif
