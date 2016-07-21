#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

struct vector *vector_make(size_t cap) {
    struct vector *v = malloc(sizeof(struct vector));
    v->capacity = cap;
    v->size = 0;
    v->data = malloc(cap * sizeof(struct coords));
    return v;
}

void free_vector(struct vector *vect) {
    free(vect->data);
    free(vect);
}

void check_size(struct vector *vect) {
    if (vect->size >= vect->capacity) {
        vect->data =
            realloc(vect->data, (vect->capacity *= 2) * sizeof(struct coords));
    }
}

void vector_push_back(struct vector *vect, struct coords x) {
    check_size(vect);
    vect->data[vect->size] = x;
    vect->size++;
}

int vector_pop_back(struct vector *vect, struct coords *x) {
    if (vect->size > 0) {
        *x = vect->data[vect->size - 1];
        vect->size--;
        return vect->size > 0;
    } else
        return 0;
}

void vector_push_front(struct vector *vect, struct coords x) {
    check_size(vect);
    vect->size++;
    for (size_t i = vect->size - 1; i > 0; i--) {
        vect->data[i] = vect->data[i - 1];
    }
    vect->data[0] = x;
}

int vector_pop_front(struct vector *vect, struct coords *x) {
    if (vect->size > 0) {
        *x = vect->data[0];
        for (size_t i = 0; i < vect->size; i++) {
            vect->data[i] = vect->data[i + 1];
        }
        vect->size--;
        return vect->size > 0;
    } else
        return 0;
}

int vector_insert_at(struct vector *vect, size_t pos, struct coords x) {
    if (pos <= vect->size) {
        check_size(vect);
        vect->size++;
        for (size_t i = vect->size; i > pos; i--) {
            vect->data[i] = vect->data[i - 1];
        }
        vect->data[pos] = x;
        return 1;
    } else
        return 0;
}

int vector_extract_at(struct vector *vect, size_t pos, struct coords *x) {
    if (vect->size == 0 || pos >= vect->size)
        return 0;
    else {
        *x = vect->data[pos];
        vect->size--;
        for (size_t i = pos; i < vect->size; i++) {
            vect->data[i] = vect->data[i + 1];
        }
        return 1;
    }
}

struct vector *vector_clone(struct vector *vect) {
    struct vector *newvect = vector_make(vect->capacity);
    for (size_t i = 0; i < vect->size; i++) {
        vector_push_back(newvect, vect->data[i]);
    }
    return newvect;
}

struct coords *vector_nth(struct vector *v, size_t n) {
    if (n >= v->size)
        return NULL;
    return v->data + n;
}

