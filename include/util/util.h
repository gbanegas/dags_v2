/*
 * util.h
 *
 *  Created on: May 30, 2018
 *      Author: vader
 */

#ifndef INCLUDE_UTIL_UTIL_H_
#define INCLUDE_UTIL_UTIL_H_

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <sodium.h>

#include "../structures/matrix_operations.h"

#include "../random/rng.h"
#include "../definitions.h"
#include "../gf/gf.h"

extern void store(matrix *src, unsigned char *dst);

extern void store_u_y(gf *v, gf *y, unsigned char *sk);

extern void random_m(unsigned char *m);

extern void random_e(const unsigned char *sigma, unsigned char *error_array);

extern void recover_public_key(const unsigned char *public_key, matrix *G);

extern void set_vy_from_sk(gf* v, gf * y, const unsigned char * sk);

extern int compute_weight(unsigned char *r, int size);

extern void permute(gf *array, int i, int length);

extern int index_of_element(const gf *v, gf element);

extern int check_positions(const int *pos, const int size);

extern gf discrete_logarithm(const gf a, const gf b);

extern void generate_int_list_of_size(int *list, int length);

#endif /* INCLUDE_UTIL_UTIL_H_ */
