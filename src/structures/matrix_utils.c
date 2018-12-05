/*
 * matrix_utils.c
 *
 *  Created on: May 9, 2018
 *      Author: vader
 */

#include "../../include/structures/matrix_utils.h"

gf* generate_vector(int size) {
	gf* vector = (gf*) calloc(size, sizeof(gf));
	return vector;
}

int* generate_vector_of_integers(int size) {
	int* vector = (int*) calloc(size, sizeof(gf));
	return vector;
}


void write_on_matrix(matrix* input_matrix, int row, int col_start, int col_end,
		gf* vector) {
	int j;
	int i = 0;

	for (j = col_start; j < col_end; j++) {
		input_matrix->data[row*input_matrix->cols + j] = vector[i];
		i++;

	}

}

void assert_values_on_matrix(int row, int start, int final, matrix *m,
		gf* vector) {
	int counter = 0;
	for (int k = start; k < final; k++) {
		m->data[row*m->cols + k] = vector[counter];
		counter++;
	}
}

int nnz(gf* arr, int size) {
	int count = 0;
	for (int i = 0; i < size; i++) {
		if (arr[i])
			count++;
	}
	return count;

}
