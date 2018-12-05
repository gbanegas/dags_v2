/*
 * quasi_dyadic_operations.c
 *
 *  Created on: May 8, 2018
 *      Author: vader
 */

#include "../../include/structures/quasi_dyadic_operations.h"

void multiply_quasi_dyadic_matrices(matrix *a, matrix *b, int rows, int cols,
		matrix *result) {
	int i, j, v;
	matrix *matrix_temp = make_matrix_with_value(rows, rows * cols, 0);
	for (i = 0; i < rows; i++) {
		for (j = 0; j < rows; j++) {
			gf* val = generate_vector(cols);
			for (v = 0; v < rows; v++) {
				gf temp[cols];
				gf temp_1[cols];
				gf term[cols];
				gf temp_2[cols];
				int count = 0;
				for (int k = (v - 1) * cols + 1; k < v * cols; k++) {
					temp[count] = a->data[i][k];
				}
				count = 0;
				for (int k = (j - 1) * cols + 1; k < j * cols; k++) {
					temp_1[count] = b->data[i][k];
				}

				for (int k = 0; k < cols; k++) {
					temp_2[k] = val[k];
				}
				dyadic_product(cols, temp, temp_1, term);
				dyadic_sum(cols, temp_2, term, val);

			}
			int count = 0;
			for (int k = (j - 1) * cols + 1; k < j * cols; k++) {
				matrix_temp->data[i][k] = val[count];
				count++;
			}
			free(val);

		}
	}

	for (i = 0; i < rows; i++) {
		for (j = 0; j < rows * cols; j++) {
			result->data[i][j] = matrix_temp->data[i][j];
		}
	}
	free_matrix(matrix_temp);

}

void quasidyadic_col_block_product(matrix *a, gf *sig_B, int k, int n,
		matrix *result) {
	int i, j, v;
	matrix *temp_matrix = make_matrix_with_value(k, n, 0);

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			int pos = i^j;
			gf element = sig_B[pos];
			for (v = 0; v < k; v++) {
				gf result_mult = gf_mult_fast(a->data[v][j], element);
				temp_matrix->data[v][i] = gf_sum(temp_matrix->data[v][i],
						result_mult);
			}
		}

	}

	for (i = 0; i < k; i++) {
		for (j = 0; j < n; j++) {
			result->data[i][j] = temp_matrix->data[i][j];
		}

	}
	free_matrix(temp_matrix);

}

void quasidyadic_invert_matrix(matrix *input_matrix, int m, int n,
		matrix *result) {
	int i, j, l;
	int temp_val = m / n;

	matrix *temp = make_matrix_with_value(temp_val, m, 1);

	for (i = 0; i < temp_val; i++) {

		gf* vector_zeros = generate_vector(n);
		write_on_matrix(temp, i, (i) * n + 1, (i + 1) * n, vector_zeros);
		int counter = 0;
		for (j = i; j < temp_val; j++) {
			gf temp_1[m * n];
			gf temp_2[m * n];
			gf signature_temp[n];
			gf dyadic_sum_result[n];
			gf signature_inverse[n];
		    gf temp_result[n];

			for (l = 0; l < j - 1; l++) {
				counter = 0;
				for (int k = (l) * n + 1; k < (l + 1) * n; k++) {
					temp_1[counter] = input_matrix->data[j][k];
					counter++;
				}
				counter = 0;
				for (int k = (i) * n + 1; k < (i + 1) * n; k++) {
					temp_2[counter] = temp->data[j][k];
					counter++;
				}
				dyadic_product(n, temp_1, temp_2, signature_temp);
				dyadic_sum(n, temp_2, signature_temp, dyadic_sum_result);
				assert_values_on_matrix(j, (i) * n, (i + 1) * n, temp,
						dyadic_sum_result);

			}

			counter = 0;
			for (int k = j * n + 1; k < (j + 1) * n; k++) {
				temp_1[counter] = input_matrix->data[j][k];
				counter++;
			}
			counter = 0;
			for (int k = (i) * n + 1; k < (i + 1) * n; k++) {
				temp_2[counter] = temp->data[j][k];
				counter++;
			}
			dyadic_inverse(n, temp_1, signature_inverse);
			dyadic_product(n, signature_inverse, temp_2, temp_result);

			assert_values_on_matrix(j, (i) * n, (i + 1) * n, temp, temp_result);

		}

		//print_matrix(temp);

	}
	for (i = 0; i < temp_val; i++) {
		for (j = 0; j < m; j++) {
			result->data[i][j] = temp->data[i][j];
		}
	}

	free_matrix(temp);
}

void quasidyadic_LU(matrix *a, int m, int n, gf* permutated_row) {

	int row_size = m / n;
	int i, j;

	//result = copy_matrix(a->data, a->rows, a->cols);
	gf* temp_perm_row = generate_vector(row_size);
	for (i = 0; i < row_size; i++) {
		temp_perm_row[i] = i;
	}
	for (j = 0; j < row_size; j++) {
		quasidyadic_pivot(a, m, n, j, temp_perm_row);
		gf temp_row[j * n];
		gf sig_temp_result[n];
		int count = 0;
		for (i = j * n + 1; i < (j + 1) * n; i++) {
			temp_row[count] = a->data[j][i];
			count++;
		}
		dyadic_inverse(n, temp_row, sig_temp_result);
		matrix* sub_a_matrix = submatrix(a, j + 1, a->rows, (j) * n + 1,
				(j + 1) * n);
		matrix* C = make_matrix((a->rows), ((j + 1) * n) - ((j) * n + 1));
		quasidyadic_col_block_product(sub_a_matrix, sig_temp_result, m / n - j,
				n, C);
		int row_count = 0;
		for (int k = j + 1; k < a->rows; k++) {
			int cols_count = 0;
			for (int y = (j) * n + 1; y < (j + 1) * n; y++) {
				a->data[k][y] = C->data[row_count][cols_count];
				cols_count++;
			}
			row_count++;
		}
		free_matrix(C);
		for (int v = j + 1; v < (m / n); v++) {
			for (int k = j + 1; k < (m / n); k++) {
				gf sig_term[n];
				gf sig_result[n];
				gf temp_a[n];
				gf temp_b[n];
				int count = 0;
				for (int x = (j) * n + 1; x < (j + 1) * n; x++) {
					temp_a[count] = a->data[v][x];
					count++;
				}
				count = 0;
				for (int x = (k) * n + 1; x < (k + 1) * n; x++) {
					temp_b[count] = a->data[v][x];
					count++;
				}
				dyadic_product(n, temp_a, temp_b, sig_term);

				dyadic_sum(n, temp_b, sig_term, sig_result);
				count = 0;
				for (int x = (k) * n + 1; x < (k + 1) * n; x++) {
					a->data[v][x] = sig_result[count];
					count++;
				}

			}
		}

	}
	for (int k = 0; k < row_size; k++) {
		permutated_row[k] = temp_perm_row[k];
	}
}

int quasidyadic_pivot(matrix *a, int m, int n, int k, gf* permutated_row) {
	int i, j;
	int flag_inv = 0;
	int temp_value = (m / n);
	for (i = k; i < temp_value; i++) {
		gf det = a->data[i][(k) * n + 1]; //(i,(k-1)*n+1);
		for (j = 1; j < n; j++) {
			det = gf_sum(det, a->data[i][n * (k) + j]); //(i,n*(k-1)+j))
		}
		if (det >= 0) {
			flag_inv = i;
			gf z = permutated_row[k];
			permutated_row[k] = permutated_row[flag_inv];
			permutated_row[flag_inv] = z;
			for (j = 0; j < m; j++) {
				gf w = a->data[k][j];
				a->data[k][j] = a->data[flag_inv][j];
				a->data[flag_inv][j] = w;
			}

		}

	}
	return flag_inv;
}
