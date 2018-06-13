/*
 * keygeneration.c
 *
 *  Created on: May 3, 2018
 *      Author: Gustavo Banegas
 */

#include "../include/keygeneration.h"

#if defined(TEST)
gf element_test[] = {54793, 14489, 9408, 3283, 63292, 64433, 54804, 26905,
	28730, 39416, 58411, 51281, 3390, 47926, 26258, 11188, 33982};
int int_vector_test[] = {16155, 8727, 7032, 6605, 10280, 11309, 12414, 3018,
	1787, 16345, 4799, 4954, 3486, 13521, 7691};
#endif

void remove_element(gf element, gf* list) {
	int i;
	for (i = 0; i < F_q_m_order; i++) {
		if (list[i] == element) {
			list[i] = 0;
			return;
		}
	}
}

void remove_element_int(int element, int* list, int lenght) {
	int i;
	for (i = 0; i < lenght; i++) {
		if (list[i] == element) {
			list[i] = 0;
			return;
		}
	}
}
int contains_zero(gf *list, int length) {
	for (int i = 0; i < length; i++) {
		if (list[i] == 0)
			return 1;
	}
	return 0;
}

void generate_elements_in_F_q_m(gf * set_of_elements_in_F_q_m) {
	for (int i = 1; i < F_q_m_size; i++) {
		set_of_elements_in_F_q_m[i - 1] = i;
	}
}

void build_dyadic_signature(gf *dyadic_signature, gf *omega) {
	gf set_of_elements_in_F_q_m[F_q_m_size] = { 0 };
	gf signature_h[F_q_m_size] = { 0 };
	int block_position[n0] = { 0 };
	gf temp_list[signature_block_size] = { 0 };

	generate_elements_in_F_q_m(set_of_elements_in_F_q_m);

	gf h0 = 0;
	h0 = set_of_elements_in_F_q_m[get_random_int(F_q_m_size)];
#if defined(TEST)
	h0 = element_test[0];
	int count = 1;
	int counter_vector_int = 0;
#endif
	gf h0_inverse = gf_q_m_inv(h0);
	signature_h[0] = h0;
	remove_element(h0, set_of_elements_in_F_q_m);

	for (int t = 0; t < extension * subfield; t++) {
		int i = 1 << t;
		gf random_e = 0;
		do {
			random_e = set_of_elements_in_F_q_m[get_random_int(F_q_m_size)];
		} while (random_e == 0);
#if defined(TEST)
		random_e = element_test[count];
		count++;
#endif
		signature_h[i] = random_e;
		remove_element(random_e, set_of_elements_in_F_q_m);
		for (int j = 1; j < i; j++) {
			if (signature_h[i] != 0 && signature_h[j] != 0) {
				gf temp = gf_q_m_inv(signature_h[i])
						^ gf_q_m_inv(signature_h[j]) ^ h0_inverse;
				if (temp != 0) {
					gf temp_inv = gf_q_m_inv(temp);
					signature_h[i + j] = temp_inv;
					remove_element(temp_inv, set_of_elements_in_F_q_m);
				} else {
					signature_h[i + j] = 0;
				}
			} else {
				signature_h[i + j] = 0;
			}
		}

	}
	int ll = 0;
	generate_elements_in_F_q_m(set_of_elements_in_F_q_m);

	for (int i = 0; i < signature_block_size; i++) {
		temp_list[i] = signature_h[i];
	}
	if (!contains_zero(temp_list, signature_block_size)) {
		block_position[0] = 0;
		ll++;
		for (int l = 0; l < signature_block_size; l++) {
			gf h_l_inverse = gf_q_m_inv(signature_h[l]);
			remove_element(h_l_inverse, set_of_elements_in_F_q_m);
			gf sum_inverses = h0_inverse ^ h_l_inverse;
			remove_element(sum_inverses, set_of_elements_in_F_q_m);
		}
		int part[(F_q_m_size / signature_block_size) - 1] = { 0 };
		generate_int_list_of_size(part,
				(F_q_m_size / signature_block_size) - 1);

		while (ll * signature_block_size < code_length) {
			int j =
					part[get_random_int((F_q_m_size / signature_block_size) - 1)];
			remove_element_int(j, part,
					(F_q_m_size / signature_block_size) - 1);
#if defined(TEST)
			j = int_vector_test[counter_vector_int];
			counter_vector_int++;
#endif
			gf aux_list[signature_block_size] = { 0 };
			for (int i = 0; i < signature_block_size; i++) {
				aux_list[i] = signature_h[(j * signature_block_size) + i];
			}
			if (!contains_zero(aux_list, signature_block_size)) {
				block_position[ll] = j;
				ll++;
				for (int i = j * signature_block_size;
						i < (j * signature_block_size) + signature_block_size;
						i++) {
					gf inverse_sum = gf_q_m_inv(signature_h[i]) ^ h0_inverse;
					remove_element(inverse_sum, set_of_elements_in_F_q_m);
				}
			}
		}
	} else {
		printf("FAIL!");
		return; //TODO: catch error
	}
	do {
		omega[0] = set_of_elements_in_F_q_m[get_random_int(F_q_m_size)];
	} while (omega[0] == 0);

	int nr_blocks = floor(F_q_m_size / signature_block_size);
	struct signature_block blocks[nr_blocks];

	for (int i = 0; i < nr_blocks; i++) {
		blocks[i].signature = (gf*) calloc(signature_block_size,  sizeof(gf)); //TODO: check to remove this malloc
	}
	int block_nr = 0;
	for (int i = 0; i < F_q_m_size; i = i + signature_block_size) {
		for (int j = 0; j < signature_block_size; j++) {
			blocks[block_nr].signature[j] = signature_h[i + j];
		}
		block_nr++;

	}

	int aux_count_transfer_block = 0;
	for (int i = 0; i < n0; i++) {
		struct signature_block block = blocks[block_position[i]];
		for (int j = 0; j < signature_block_size; j++) {
			gf value = block.signature[j];
			dyadic_signature[aux_count_transfer_block] = value;
			aux_count_transfer_block++;
		}
	}

	for (int i = 0; i < nr_blocks; i++) {
		free(blocks[i].signature);
	}

}

void build_support(gf omega, gf *signature_h, gf *u, gf *v) {

	gf h0_inv = gf_q_m_inv(signature_h[0]);

	for (int i = 0; i < code_length; i++) {
		if (signature_h[i] != 0) {
			gf sum_inv = gf_q_m_inv(signature_h[i]) ^ h0_inv ^ omega;
			v[i] = sum_inv;
		}
	}

	for (int i = 0; i < signature_block_size; i++) {
		if (signature_h[i] != 0) {
			gf sum_inv = gf_q_m_inv(signature_h[i]) ^ omega;
			u[i] = sum_inv;
		}
	}

}

void build_cauchy_matrix(gf *u, gf *v, matrix *H_cauchy) {

	matrix *H = make_matrix(signature_block_size, code_length);
	for (int i = 0; i < signature_block_size; i++) {
		for (int j = 0; j < code_length; j++) {
			gf inv = gf_q_m_inv((u[i] ^ v[j]));
			H->data[i * H->cols + j] = inv;
		}
	}

	for (int k = 1; k < extension + 1; k++) {
		for (int i = 0; i < signature_block_size; i++) {
			for (int j = 0; j < code_length; j++) {
				gf result = gf_pow_f_q_m((H->data[i * H->cols + j]), k);
				H_cauchy->data[((k - 1) * signature_block_size + i) * H->cols
						+ j] = result;

			}
		}
	}
	free_matrix(H);
}

void build_trapdoor(const matrix *H_cauchy, const gf *v, const gf *u, gf *y,
		matrix *H) {
#if defined(TEST)
	gf z[] = {65011, 65011, 65011, 65011, 53089, 53089, 53089, 53089, 53419,
		53419, 53419, 53419, 49545, 49545, 49545, 49545, 1425, 1425, 1425,
		1425, 50395, 50395, 50395, 50395, 36862, 36862, 36862, 36862, 56849,
		56849, 56849, 56849, 48210, 48210, 48210, 48210, 25635, 25635,
		25635, 25635, 4586, 4586, 4586, 4586, 36597, 36597, 36597, 36597,
		28058, 28058, 28058, 28058, 33475, 33475, 33475, 33475, 19604,
		19604, 19604, 19604, 24979, 24979, 24979, 24979};
#endif
#if defined(RUN)
	gf z[code_length] = { 0 };
	gf set_of_elements_in_F_q_m[F_q_m_size] = { 0 };

	generate_elements_in_F_q_m(set_of_elements_in_F_q_m);

	for (int i = 0; i < n0; i++) {
		z[i * signature_block_size] = set_of_elements_in_F_q_m[get_random_int(
		F_q_m_size)];
		for (int j = 1; j < signature_block_size; j++) {
			z[(i * signature_block_size) + j] = z[i * signature_block_size];
		}
	}
#endif
	matrix *diagonal_z_matrix = diagonal_matrix(z, code_length, code_length);
	matrix *H3 = matrix_multiply(H_cauchy, diagonal_z_matrix);

	for (int i = 0; i < H3->rows; i++) {
		for (int j = 0; j < H3->cols; j++) {
			H->data[i * H->cols + j] = H3->data[i * H->cols + j];
		}
	}
	free_matrix(H3);
	free_matrix(diagonal_z_matrix);

	for (int i = 0; i < code_length; i++) {
		gf pol = 1;
		for (int j = 0; j < signature_block_size; j++) {
			gf sum_u_v = (v[i] ^ u[j]);
			gf result = gf_pow_f_q_m(sum_u_v, extension);
			pol = gf_q_m_mult(pol, result);
		}
		gf div = gf_div_f_q_m(z[i], pol);
		y[i] = div;
	}
}

void project_H_on_F_q(const matrix *H, matrix *Hbase) {
	int nr_rows = H->rows;
	int nr_cols = H->cols;
	for (int k = 0; k < extension; k++) {
		for (int i = 0; i < nr_rows; i++) {
			for (int j = 0; j < nr_cols; j++) {
				gf element_in_f_q_m = H->data[i * H->cols + j];
				gf element_in_f_q = relative_field_representation(
						element_in_f_q_m, k);
				Hbase->data[(k * nr_rows + i) * H->cols + j] = element_in_f_q;
			}
		}
	}

}

matrix* generate_systematic_matrix(const matrix* Hbase) {
	int n = Hbase->cols;
	int r = Hbase->rows;

	matrix * aux_A = submatrix(Hbase, 0, 0, r, (n - r));

	matrix * aux_B = submatrix(Hbase, 0, (n - r), r, r);

	matrix * aux_H = augment(aux_B, aux_A);
	free_matrix(aux_A);
	free_matrix(aux_B);

	echelon_form(aux_H);

	matrix *matrix_M = submatrix(aux_H, 0, r, r, (n - r));

	free_matrix(aux_H);
	matrix *m_temp = make_matrix(r, r);
	for (int i = 0; i < r; i++)
		m_temp->data[i * r + i] = 1;

	matrix *H = augment(matrix_M, m_temp);
	free_matrix(matrix_M);
	free_matrix(m_temp);

	return H;
}

void generate_public_key(const matrix *Hbase, matrix *G) {

	matrix *HH = generate_systematic_matrix(Hbase);
	int n = Hbase->cols;
	int r = Hbase->rows;

	matrix *M = submatrix(HH, 0, 0, r, (n - r));

	free_matrix(HH);

	matrix *m_temp = make_matrix((n - r), (n - r));
	for (int i = 0; i < (n - r); i++)
		m_temp->data[i * (n - r) + i] = 1;

	matrix *m_transposed = transpose_matrix(M);

	free_matrix(M);

	matrix *final = augment(m_temp, m_transposed);
	free_matrix(m_temp);
	free_matrix(m_transposed);

	for (int i = 0; i < final->rows; i++) {
		for (int j = 0; j < final->cols; j++) {
			G->data[i * G->cols + j] = final->data[i * final->cols + j];
		}
	}
	free_matrix(final);

}

int key_pair_generation(unsigned char *pk, unsigned char *sk) {
	gf v[code_length] = { 0 };
	gf y[code_length] = { 0 };
	matrix G;
	G.cols = code_length;
	G.rows = code_length - ((signature_block_size * extension) * extension);
	gf data_G[code_length
			* (code_length - ((signature_block_size * extension) * extension))] =
			{ 0 };
	G.data = data_G;
	key_gen(v, y, &G);
	store(&G, pk);
	store_u_y(v, y, sk);

	return 0;
}

void key_gen(gf *v, gf *y, matrix *G) {
	gf signature_h[code_length] = { 0 };
	gf u[signature_block_size] = { 0 };
	gf omega[1] = { 0 };

	build_dyadic_signature(signature_h, omega);

	build_support(omega[0], signature_h, u, v);

	matrix H_cauchy;
	H_cauchy.rows = signature_block_size * extension;
	H_cauchy.cols = code_length;
	gf data_cauchy[signature_block_size * extension * code_length] = { 0 };
	H_cauchy.data = data_cauchy;
	build_cauchy_matrix(u, v, &H_cauchy);

	matrix H;
	H.rows = signature_block_size * extension;
	H.cols = code_length;
	gf data_H[signature_block_size * extension * code_length] = { 0 };
	H.data = data_H;
	build_trapdoor(&H_cauchy, v, u, y, &H);

	matrix Hbase;
	Hbase.rows = (signature_block_size * extension) * extension;
	Hbase.cols = code_length;
	gf data_Hbase[signature_block_size * extension * code_length * extension] =
			{ 0 };
	Hbase.data = data_Hbase;
	project_H_on_F_q(&H, &Hbase);

	generate_public_key(&Hbase, G);

}
