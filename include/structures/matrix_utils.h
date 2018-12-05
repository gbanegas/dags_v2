/*
 * matrix_utils.h
 *
 *  Created on: May 9, 2018
 *      Author: vader
 */

#ifndef SRC_MATRIX_UTILS_H_
#define SRC_MATRIX_UTILS_H_

#include <stdlib.h>

#include "random_generator.h"
#include "definitions.h"
#include "param.h"

extern gf* generate_vector(int size);
extern int* generate_vector_of_integers(int size);

extern void write_on_matrix(matrix* input_matrix, int row, int col_start,
		int col_end, gf* vector);

extern void assert_values_on_matrix(int row, int start, int final, matrix *m,
		gf* vector);

extern int nnz(gf* arr, int size);

#endif /* SRC_MATRIX_UTILS_H_ */
