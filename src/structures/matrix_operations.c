/*
 * matrix_operations.c
 *
 *  Created on: May 4, 2018
 *      Author: vader
 */

#include "../../include/structures/matrix_operations.h"

#define min(a,b) (((a)<(b))?(a):(b))

matrix* make_matrix(int n_rows, int n_cols) {

	matrix * m = (matrix *) malloc(sizeof(matrix));
	// set dimensions
	m->rows = n_rows;
	m->cols = n_cols;

	// allocate a double array of length rows * cols
	m->data = (gf *) malloc(n_rows * n_cols * sizeof(gf));
	// set all data to 0
	int i;
	for (i = 0; i < n_rows * n_cols; i++)
		m->data[i] = 0;

	return m;
}

matrix* diagonal_matrix(gf* z, int n_rows, int n_cols) {
	int i;
	matrix * m = (matrix *) malloc(sizeof(matrix));

	// set dimensions
	m->rows = n_rows;
	m->cols = n_cols;

	// allocate a double array of length rows * cols
	m->data = (gf *) malloc(n_rows * n_cols * sizeof(gf));
	// set all data to 0
	for (i = 0; i < n_rows * n_cols; i++)
		m->data[i] = 0;

	int count = 0;

	for (i = 0; i < min(n_rows, n_cols); i++){
		m->data[i * n_cols + i] = z[count];
		count++;
	}
	return m;
}

matrix* matrix_multiply(const matrix *a, const matrix *b) {
	matrix *m = make_matrix(a->rows, b->cols);
	mxm_product(m->data, a->data, b->data, a->rows, a->cols, b->cols);
	return m;

}

void free_matrix(matrix* mtx) {

	assert(mtx->data);
	free(mtx->data);
	free(mtx);
}

matrix* submatrix(const matrix* m, int i, int j, int nr_row, int nr_col) {

	matrix* m_new = make_matrix(nr_row, nr_col);
	int j_temp = j;

	int new_row = 0;
	for (int row = 0; row < nr_row; row++) {
		int new_col = 0;
		j_temp = j;
		for (int col = 0; col < nr_col; col++) {
			m_new->data[new_row * nr_col + new_col] = m->data[i * m->cols
					+ j_temp];
			new_col++;
			j_temp++;
		}
		i++;
		new_row++;
	}
	return m_new;

}

matrix* augment(const matrix *a, const matrix *b) {
	int n_rows = a->rows;
	int n_cols = a->cols + b->cols;
	matrix *result = make_matrix(n_rows, n_cols);

	for (int i = 0; i < n_rows; i++) {
		for (int j = 0; j < a->cols; j++) {
			result->data[result->cols * i + j] = a->data[a->cols * i + j];
		}
		for (int j = a->cols; j < n_cols; j++) {
			result->data[result->cols * i + j] = b->data[(b->cols * i)
					+ (j - a->cols)];
		}
	}
	return result;
}

void echelon_form(matrix *a) {
	int nrows = a->rows;
	int ncols = a->cols;

	int lead = 0;

	while (lead < nrows) {
		float d, m;

		for (int r = 0; r < nrows; r++) { // for each row ...
			/* calculate divisor and multiplier */
			d = a->data[lead * ncols + lead];
			m = gf_div(a->data[r * ncols + lead], a->data[lead * ncols + lead]);

			for (int c = 0; c < ncols; c++) { // for each column ...
				if (r == lead)
					a->data[r * ncols + c] = gf_div(a->data[r * ncols + c], d); // make pivot = 1
				else
					a->data[r * ncols + c] ^= gf_mult(a->data[lead * ncols + c],
							m); // make other = 0
			}
		}

		lead++;

	}


}

matrix * transpose_matrix(matrix *a) {

	int n_a_cols = a->cols;
	int n_a_rows = a->rows;
	matrix* transposed = make_matrix(n_a_cols, n_a_rows);

	for (int c = 0; c < n_a_rows; c++)
		for (int d = 0; d < n_a_cols; d++)
			transposed->data[d * n_a_rows + c] = a->data[c * n_a_cols + d];

	return transposed;
}

void print_matrix(matrix* m) {
	int i, j;

	for (i = 0; i < m->rows; i++) {
		printf("| ");
		for (j = 0; j < m->cols; j++)
			printf(" %" PRIu16 " ", m->data[i * m->cols + j]);
		printf(" |\n");
	}
	printf("\n");
}

void multiply_vector_matrix(const unsigned char* u, matrix *G, gf *c) {

	int i, k;
	for (i = 0; i < G->cols; i++) {
		for (k = 0; k < G->rows; k++) {
			c[i] ^= gf_mult(G->data[k * G->cols + i], u[k]);
		}
	}

}

void quasi_dyadic_bloc_matrix(matrix *M, gf *sig, int ind_col, int ind_rown) {
	int i, j;
	for (i = ind_rown; i < signature_block_size + ind_rown; i++) {
		for (j = ind_col; j < signature_block_size + ind_col; j++) {
			M->data[i * M->cols + j] = sig[(i ^ j) % signature_block_size];
		}
	}
}
