/*
 * util_debug.c
 *
 *  Created on: Dec 20, 2018
 *      Author: vader
 */

#ifndef SRC_UTIL_UTIL_DEBUG_C_
#define SRC_UTIL_UTIL_DEBUG_C_
#include "../../include/util/util_debug.h"


#ifdef DEBUG
void store_matrix_in_file(matrix *m){
	int i, j;

    FILE *matrix=fopen("matrix.txt", "w");
    fprintf(matrix,"[ ");
	for (i = 0; i < m->rows; i++) {
		fprintf(matrix,"[ ");
		for (j = 0; j < m->cols; j++)
			fprintf(matrix, " %" PRIu16 ", ", m->data[i * m->cols + j]);
		fprintf(matrix," ],\n");
	}
	fprintf(matrix,"]");
	fprintf(matrix,"\n");
	fclose(matrix);
}

void store_vector_in_file(gf *v, int size){
	int i;

    FILE *vector=fopen("vector.txt", "w");
    fprintf(vector,"[ ");
	for (i = 0; i < size; i++) {

			fprintf(vector, " %" PRIu16 ",", v[i]);
	}
	fprintf(vector,"]");
	fprintf(vector,"\n");
	fclose(vector);
}

void store_char_vector_in_file(unsigned char *v, int size){
	int i;

    FILE *vector=fopen("vector_u.txt", "w");
    fprintf(vector,"[ ");
	for (i = 0; i < size; i++) {

			fprintf(vector, " %" PRIu8 ",", v[i]);
	}
	fprintf(vector,"]");
	fprintf(vector,"\n");
	fclose(vector);
}

#endif

#endif /* SRC_UTIL_UTIL_DEBUG_C_ */
