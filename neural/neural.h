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


#endif
