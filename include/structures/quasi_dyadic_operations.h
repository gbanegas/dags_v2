/*
 * quasi_dyadic_operations.h
 *
 *  Created on: May 8, 2018
 *      Author: vader
 */

#ifndef SRC_QUASI_DYADIC_OPERATIONS_H_
#define SRC_QUASI_DYADIC_OPERATIONS_H_

#include <stdlib.h>
#include <math.h>

#include "param.h"
#include "matrix_utils.h"
#include "matrix_operations.h"
#include "definitions.h"
#include "dyadic_operations.h"
#include "gf/gf_6.h"

extern void multiply_quasi_dyadic_matrices(matrix *a, matrix *b, int rows,
		int cols, matrix *result);

extern void quasidyadic_col_block_product(matrix *A, gf *sig_B, int k, int n,
		matrix *result);

extern void quasidyadic_invert_matrix(matrix *l, int m, int n, matrix *result);

extern void quasidyadic_LU(matrix *a, int m, int n, gf* permutated_row);

extern int quasidyadic_pivot(matrix *a, int m, int n, int k, gf* perm_row);

#endif /* SRC_QUASI_DYADIC_OPERATIONS_H_ */
