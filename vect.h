#ifndef VECT_H
#define VECT_H

#include <stdlib.h>
#include <stdio.h>

struct yeetVec {
    double x;
    double y;
    double z;
    double w;
};

struct vec {
    int dimension;
    union {
        double *elements;
        struct yeetVec *e;
    };
};

struct mat2 {
    int num_rows;
    int num_cols;
    double **elements;
};

struct vec* new_vec(int num_dimensions);
struct vec* new_random_vec(int num_dimensions, double min, double max);
struct vec* new_vec_of(int num_dimensions, double value);

void free_vec(struct vec*);

struct vec* new_vec2(double x, double y);
struct vec* new_vec3(double x, double y, double z);
struct vec* new_vec4(double w, double x, double y, double z);

struct vec* add_vec(struct vec* a, struct vec* b);
struct vec* add_vec_ip(struct vec* a, struct vec* b);
struct vec* add_scaled_vec_ip(struct vec* a, struct vec* b, double scaleFactor);

struct vec* subtract_vec(struct vec* a, struct vec* b);
struct vec* subtract_vec_ip(struct vec* a, struct vec* b);

struct vec* normalise_vec(struct vec* a);
struct vec* normalise_vec_ip(struct vec* a);

double magnitude_vec(struct vec* a);
double distance_vec(struct vec* a, struct vec* b);

double dot_product_vec(struct vec* a, struct vec* b);

struct vec* scalar_multiply_vec(struct vec* a, double multiplier);
struct vec* scalar_multiply_vec_ip(struct vec* a, double multiplier);

struct vec* copy_vec(struct vec*);

struct vec* perpendicular_vec(int num_vectors, struct vec** vectors);

/* do something to every value in a vector */
struct vec* do_on_vec_ip(struct vec * v, double (*func)(double));

/* get the smallest value in the vector */
double vec_min(const struct vec *v); 

/* get the largest value in the vector */
double vec_max(const struct vec *v); 

#endif
