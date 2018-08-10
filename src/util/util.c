/*
 * util.c
 *
 *  Created on: May 30, 2018
 *      Author: vader
 */

#include "../include/util/util.h"

void store(matrix *src, unsigned char *dst) {
	int i, j, k, p, d, a = 0;
	k = code_dimension / (signature_block_size);
	p = (code_length - code_dimension) / 4;
	gf c1 = 0, c2 = 0, c3 = 0, c4 = 0;
	unsigned char c = 0;

	for (i = 0; i < k; i++) {
		d = i * (signature_block_size);
		for (j = 0; j < p; j++) {
			c1 = src->data[4 * j * src->cols + d];
			c2 = src->data[(4 * j + 1) * src->cols + d];
			c3 = src->data[(4 * j + 2) * src->cols + d];
			c4 = src->data[(4 * j + 3) * src->cols + d];
			c = (c1 << 2) ^ (c2 >> 4);
			//printf("--c= %d \t",c);
			dst[a] = c;
			a += 1;
			c1 = (c2 & 15);
			c = (c1 << 4) ^ (c3 >> 2);
			//printf("--c= %d \t",c);
			dst[a] = c;
			a += 1;
			c1 = (c3 & 3);
			c = (c1 << 6) ^ c4;
			//printf("--c= %d \t",c);
			dst[a] = c;

			a += 1;
		}
		//affiche_vecteur(L,code_length-code_dimension);
		//printf(" \n");
	}
}

void store_u_y(gf *v, gf *y, unsigned char *sk) {
	int i, a = 0;
	gf c1, c2;
	unsigned char c;

	for (i = 0; i < (code_length / 2); i++) {
		c1 = v[2 * i];
		c2 = v[2 * i + 1];
		c = c1 >> 4;
		sk[a] = c;
		a += 1;
		c1 = c1 & 15;
		c = (c1 << 4) ^ (c2 >> 8);
		sk[a] = c;
		a += 1;
		c = c2 & 255;
		sk[a] = c;
		a += 1;
	}
	for (i = 0; i < (code_length / 2); i++) {
		c1 = y[2 * i];
		c2 = y[2 * i + 1];
		c = c1 >> 4;
		sk[a] = c;
		a += 1;
		c1 = c1 & 15;
		c = (c1 << 4) ^ (c2 >> 8);
		sk[a] = c;
		a += 1;
		c = c2 & 255;
		sk[a] = c;
		a += 1;
	}

}

void recover_public_key(const unsigned char *public_key, matrix *G) {
	int a = 0;
	int i, j, k, p, l, m, q;
	matrix *M = make_matrix(code_dimension, code_length - code_dimension);
	k = code_dimension / (signature_block_size);
	p = (code_length - code_dimension) / 4;
	gf c1 = 0, c2 = 0, c3 = 0, c4 = 0, tmp1 = 0, tmp2 = 0;
	q = (code_length - code_dimension) / (signature_block_size);
	unsigned char c = 0;
	gf sig[signature_block_size] = { 0 };
	gf signature_all_line[(code_length - code_dimension)] = { 0 };
	for (i = 0; i < k; i++) {
		for (j = 0; j < p; j++) {
			c = public_key[a];
			//printf("--c= %d \t",c);
			c1 = c >> 2;
			signature_all_line[4 * j] = c1;
			tmp1 = (c & 3);
			a += 1;
			c = public_key[a];
			//printf("--c= %d \t",c);
			c2 = (tmp1 << 4) ^ (c >> 4);
			signature_all_line[4 * j + 1] = c2;
			tmp2 = c & 15;
			a += 1;
			c = public_key[a];
			a += 1;
			//printf("--c= %d \t",c);
			c3 = (tmp2 << 2) ^ (c >> 6);
			signature_all_line[4 * j + 2] = c3;
			c4 = c & 63;
			signature_all_line[4 * j + 3] = c4;
		}
		for (l = 0; l < q; l++) {
			for (m = 0; m < (signature_block_size); m++) {
				sig[m] = signature_all_line[l * (signature_block_size) + m];
			}
			//affiche_vecteur(sig,order);
			quasi_dyadic_bloc_matrix(M, sig, l * (signature_block_size),
					i * (signature_block_size));
		}
	}
	for (i = 0; i < G->rows; i++) {
		G->data[i * G->cols + i] = 1;
		for (j = M->rows; j < G->cols; j++) {
			G->data[i * G->cols + j] = M->data[(i * M->cols) + j - M->rows];
		}
	}
	free_matrix(M);
}

void generate_int_list_of_size(int *list, int length) {
	for (int i = 0; i < length; i++) {
		list[i] = i;
	}

}

void random_m(unsigned char *m) {
	const unsigned char seed[32U] = { 1 };
	randombytes_buf_deterministic(m, k_prime, seed);
	//randombytes(m, k_prime);
	/*unsigned char data_m[] = { 114, 194, 6, 152, 244, 38, 43, 140, 189, 83, 66,
	 48 };
	 for (i = 0; i < k_prime; i++) {
	 m[i] = data_m[i] & F_q_order;
	 }*/
}

int indice_in_vec(unsigned int *v, int j, int size) {
	int i;
	for (i = 0; i < size; i++) {
		if (v[i] == j)
			return 1;
	}
	return 0;
}

void random_e(const unsigned char *sigma, unsigned char *error_array) {
	/*unsigned char error[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 247, 0, 0, 0, 181, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0,
	 0, 0, 0, 0, 0 };
	 for (int i = 0; i < code_length; i++) {
	 error_array[i] = error[i];
	 }*/

	int i, j = 0, k = 0, jeton = 0;
	unsigned int v[code_length] = { 0 };
#ifdef DEBUG_P
	printf("error_position: ");
#endif
	for (i = 0; i < code_length; i++) {
		if (sigma[i] % F_q_size == 0) {
			continue;
		}
		if (j == weight) {
			break;
		}
		do {
			jeton = (sigma[k + 1] ^ (sigma[k] << 4)) % code_length;
			k++;
		} while (indice_in_vec(v, jeton, j + 1) == 1); //Only check j elements
		v[j] = jeton;
		error_array[jeton] = sigma[i] % F_q_size;
#ifdef DEBUG_P
		printf("%d, ", jeton);
#endif
		jeton = 0;
		j++;
	}
#ifdef DEBUG_P
	printf("\n");
	for (int i = 0; i < code_length; i++) {
		printf("%d, ", error_array[i]);
	}
	printf("\n");
#endif
}

void set_vy_from_sk(gf* v, gf * y, const unsigned char * sk) {
	int i, a = 0;
	unsigned char c;
	gf c1, c2, c3;
	for (i = 0; i < (code_length / 2); i++) {
		c = sk[a];
		c1 = c;
		a += 1;
		c = sk[a];
		c2 = c;
		a += 1;
		c = sk[a];
		c3 = c;
		a += 1;
		v[2 * i] = (c1 << 4) ^ (c2 >> 4);
		c1 = c2 & 15;
		v[2 * i + 1] = (c1 << 8) ^ c3;
	}
	for (i = 0; i < (code_length / 2); i++) {
		c = sk[a];
		c1 = c;
		a += 1;
		c = sk[a];
		c2 = c;
		a += 1;
		c = sk[a];
		c3 = c;
		a += 1;
		y[2 * i] = (c1 << 4) ^ (c2 >> 4);
		c1 = c2 & 15;
		y[2 * i + 1] = (c1 << 8) ^ c3;
	}

}

int compute_weight(unsigned char *vector, int size) {
	int i = 0, w = 0;
	for (i = 0; i < size; i++) {
		if (vector[i] != 0)
			w++;
	}
	return w;
}

int index_of_element(const gf *v, gf element) {
	for (int i = 0; i < code_length; i++) {
		if (v[i] == element) {
			return i;
		}
	}
	return -1;

}

void swap(gf* str, int i, int j) {
	char temp = str[i];
	str[i] = str[j];
	str[j] = temp;
}
void permute(gf *array, int i, int length) {
	int j = i;
	for (j = i; j < length; j++) {
		swap(array, i, j);
		permute(array, i + 1, length);
		swap(array, i, j);
	}
	return;
}

int check_positions(const int *pos, const int size) {
	int i = 0;
	for (i = size; i > -1; i--) {
		if (pos[i] == -1) {
			return 1;
		}
	}
	return 0;
}

int multiplicative_order(const int a) {
	int order = 1;
	gf result = gf_pow_f_q_m(a, order);
	while (result != 1) {
		order++;
		result = gf_pow_f_q_m(a, order);

	}
	return order;
}
gf discrete_logarithm(const gf a, const gf b) {
	int p = multiplicative_order(b); // multiplicative order of base
	gf y = a;

	int N = sqrt(p) + 1;
	gf tbl[N];
	for (int i = 0; i <= N; i++) {
		tbl[i] = gf_pow_f_q_m(b, i);
	}
	gf temp = gf_pow_f_q_m(b, N);
	gf inv_a_m = gf_q_m_inv(temp);
	for (int j = 0; j < N; j++) {
		for (int i = 0; i < N; i++) {
			if (y == tbl[i]) {
				gf result = i + (N * j);

				return result;
			}
		}
		y = gf_q_m_mult(y, inv_a_m);
	}

	return -1;
}
