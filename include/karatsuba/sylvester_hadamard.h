/*
 * sylvester_hadamard.h
 *
 *  Created on: May 23, 2018
 *      Author: vader
 */

#ifndef SRC_SYLVESTER_HADAMARD_H_
#define SRC_SYLVESTER_HADAMARD_H_

#include <stdio.h>

#include "../structures/matrix_operations.h"
#include "../definitions.h"
#include "../gf/gf.h"
#include "../parameters/param.h"

extern void sylvester_hadamard_multiplication(int n, const int *a, int * result);

extern void multiply_dyadic_convolution(int n, const int* a, const int* b, int* result);

extern void sylvester_hadamard_multiplication_polynomial(const int poly_length, const int n, const int *a, int * result);

extern void multiply_dyadic_convolution_gf(int n, const gf* a, const gf* b, gf* result);

extern matrix *make_walsh_hadamard_matrix(int n);


#endif /* SRC_SYLVESTER_HADAMARD_H_ */
