#ifndef WEIGHT_FILE_H_INCLUED
#define WEIGHT_FILE_H_INCLUED

#include "neural.h"

int save_network_to_file(struct network *net, char *filename);
struct network *load_network_from_file(char *filename);

#endif
