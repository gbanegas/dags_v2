/*
 * matrix_operations.h
 *
 *  Created on: May 4, 2018
 *      Author: vader
 */

#ifndef INCLUDE_STRUCTURES_MATRIX_OPERATIONS_H_
#define INCLUDE_STRUCTURES_MATRIX_OPERATIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "mxm.h"

#include "../definitions.h"
#include "../gf/gf.h"

extern matrix* make_matrix(int n_rows, int n_cols);

extern matrix* diagonal_matrix(gf* data, int n_rows, int n_cols);

extern void free_matrix(matrix* m);

extern void print_matrix(matrix* m);

extern matrix* matrix_multiply(const matrix* a,const  matrix* b);

extern matrix* submatrix(const matrix* m, int star_row, int end_row, int start_column, int end_column);

extern matrix* augment(const matrix *a, const matrix *b);

extern void echelon_form(matrix *a);

extern matrix* transpose_matrix(matrix *a);

extern void multiply_vector_matrix(const unsigned char* u, matrix *G, gf *c);

extern void quasi_dyadic_bloc_matrix( matrix *M, gf *sig, int ind_col, int ind_rown);

extern gf matrix_determinant(matrix *a);

extern matrix* make_matrix_with_value(int n_rows, int n_cols, gf value);



#endif /* INCLUDE_STRUCTURES_MATRIX_OPERATIONS_H_ */
