/* vector.h : Simple vector implementation */

#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdlib.h>

struct coords {
    int w1, w2;
    int h1, h2;
};

struct vector {
    size_t capacity, size;
    struct coords *data;
};


struct vector *vector_merge(struct vector *v1, struct vector *v2);

/*
 * vector_make(capacity) create an empty vector
 * with initial storage size capacity
 */
struct vector *vector_make(size_t capacity);

void free_vector(struct vector *vect);
/*
 * vector_push_back(vect, x) add x at the end of vect
 * if vect is full increase capacity
 */
void vector_push_back(struct vector *vect, struct coords x);

/*
 * vector_pop_back(vect, &x) get the last element of vect
 * remove the element
 * store result in x
 * return true (!=0) if size > 0
 * return false (0) otherwise
 */
int vector_pop_back(struct vector *vect, struct coords *x);

/*
 * vector_push_front(vect, x) add x at the beginning of vect
 * if vect is full increase capacity
 */
void vector_push_front(struct vector *vect, struct coords x);

/*
 * vector_pop_back(vect, &x) get the first element of vect
 * remove the element
 * store result in x
 * return true (!=0) if size > 0
 * return false (0) otherwise
 */
int vector_pop_front(struct vector *vect, struct coords *x);

/*
 * vector_insert_at(vect, pos, x) add x in pos cell of vect
 * return true (!=0) if pos <= size of vect
 * return false (0) otherwise
 * vector_insert_at(v, v->size, x) is equivalent to vector_push_back(v, x)
 * vector_insert_at(v, 0, x) is equivalent to vector_push_front(v, x)
 * if vect is full increase capacity
 */
int vector_insert_at(struct vector *vect, size_t pos, struct coords x);

/*
 * vector_extract_at(vect, pos, &x) get the pos element of vect
 * remove the element
 * store result in x
 * return false (0) if size == 0 or pos >= size
 * vector_extract_at(v, v->size - 1, &x) is equivalent to vector_pop_back(v, &x)
 * vector_extract_at(v, 0, &x) is equivalent to vector_pop_front(v, &x)
 */
int vector_extract_at(struct vector *vect, size_t pos, struct coords *x);

/*
 * vector_clone(vect) create a complete copy of vect
 * both storage contains the same elements but are not at the same memory place
 */
struct vector *vector_clone(struct vector *vect);

struct coords *vector_nth(struct vector *v, size_t n);

#endif
