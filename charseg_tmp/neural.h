#ifndef NEURAL_H_INCLUED
#define NEURAL_H_INCLUED

struct neuron {
    double *w;   // tableau de poids
    double *lw;
    size_t nw;   // nbs de poids
    double b;    // bias
    double lder; // dernière valeur des dériver (error_der * sig_der)
    double lout; // dernière sortie
    double lin; //dernière entrée
};

struct network {
    size_t *L;         // tailles des layers
    size_t nL;         // nombre de layers
    struct neuron **n; // tableau de neurone (L[i] * nl);
};

struct try {
        double *in; // essai entrée
        double *res; // essai resultat attendu
};


struct neuron init_neuron(size_t nw, int ent) ;
void free_network_neurons(struct network *net) ;
void feedforward(struct network *n, double *in) ;
double *get_out(struct network net);
int *get_bin_out(struct network net) ;

struct try
    *init_numbers_0_to_9(char *path) ;
struct network init_network(size_t *L, size_t nL) ;
void free_trys(struct try *tries, size_t number) ;
void backpropagation(struct network *n, double k, struct try in);
void train(struct network *net, struct try *tr, size_t nbval, size_t nite,
           size_t display) ;
#endif
