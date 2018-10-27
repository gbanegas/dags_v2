/*
 * keygeneration.c
 *
 *  Created on: May 3, 2018
 *      Author: Gustavo Banegas
 */

#include "../include/keygeneration.h"
#include "time.h"

static int build_dyadic_signature_part_1(gf *signature_h);
static int build_dyadic_signature_part2(gf *signature_h, int * block_position);
static int build_dyadic_signature_part3(gf *signature_h, int * block_position, gf *dyadic_signature);

/*
 * contains_zero:
 * 	Check to see if the array does not contain zeros
 * exit:
 * 	Return EXIT_SUCCESS if the array does not contain zeros
 * 	and EXIT_FAILURE if the array does contain zeros.
 */

int contains_zero(gf *list, int length) {
	for (int i = 0; i < length; i++) {
		if (list[i] == 0)
			return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

/*
 * generate_elements_in_F_q_m:
 * 	The list that is generated is a list of elements 1,2,3 ...
 * 	all the way up to F_q_m
 */
void generate_elements_in_F_q_m(gf * set_of_elements_in_F_q_m) {
	int i;
	for (i = 1; i < F_q_m_size; i++) {
		set_of_elements_in_F_q_m[i - 1] = i;
	}
}

/*
 * remove_integer:
 * 	This functions goes through a list and sets the element that is equal
 * 	to the @element variable to -1
 *
 * Function returns EXIT_SUCCESS if the if the element was found and set to -1
 * otherwise EXIT_FAILURE
 */
int remove_integer(int element, int *list, int size) {
	int i;
	for (i = 0; i < size; i++) {
		if (list[i] == element) {
			list[i] = -1;
			return EXIT_SUCCESS;
		}
	}
	return EXIT_FAILURE;
}

/*
 * This function goes through and checks all of the entries in the length to make
 * sure that they do not contain @random_e up to length.
 * Returns:
 * 	EXIT_SUCCESS if the element is not present
 * 	EXIT_FAILURE if the element is present
 */
int vector_contains(gf *signature_h, gf random_e, int length) {
	int i;
	for (i = 0; i < length; i++) {
		if (signature_h[i] == random_e)
			return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int build_dyadic_signature(gf *dyadic_signature) {
	PRINT_DEBUG("Build_dyadic_signature start\n");

	int block_position[n0] = { 0 };
	gf signature_h[F_q_m_size] = { 0 };

	if (EXIT_SUCCESS != build_dyadic_signature_part_1(&signature_h))
	{
		PRINT_DEBUG("build_dyadic_signature_part_1 failed\n");
		return EXIT_FAILURE;
	}

	if(EXIT_SUCCESS != build_dyadic_signature_part2(&signature_h, &block_position))
	{
		PRINT_DEBUG("build_dyadic_signature_part_2 failed\n");
		return EXIT_FAILURE;
	}

	if (EXIT_SUCCESS != build_dyadic_signature_part3(&signature_h, &block_position, dyadic_signature))
	{
		PRINT_DEBUG("build_dyadic_signature_part_3 failed\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;

}

static int build_dyadic_signature_part_1(gf *signature_h)
{
	gf h0 = 0, h0_inverse, i_inverse;
	int t, i, j; //for loop variables
	gf random_e, temp, temp_inv;

	//Set h0 to anything but 0
	do {
		h0 = randombytes_uniform(F_q_m_size - 1);
	} while (h0 == 0);

	h0_inverse = gf_q_m_inv(h0);

	signature_h[0] = h0;
	for (t = 0; t < extension * subfield; t++) {
		i = 1 << t;
		random_e = 0;

		//random_e must be not in signature_h or equal 0
		do {
			random_e = randombytes_uniform(F_q_m_size - 1);
		} while (random_e == 0
				|| vector_contains(signature_h + i, random_e, code_length));

		//Set signature_h[1<<t] to random_e
		signature_h[i] = random_e;
		//For loop through values from 1 to i setting signature_h values to the
		i_inverse = gf_q_m_inv(signature_h[i]);
		for (j = 1; j < i; j++) {
			if (signature_h[i] != 0 && signature_h[j] != 0) {
				temp = i_inverse ^ gf_q_m_inv(signature_h[j]) ^ h0_inverse;
				if (temp != 0) {
					temp_inv = gf_q_m_inv(temp);
					signature_h[i + j] = temp_inv;
				} else {
					signature_h[i + j] = 0;
				}
			} else {
				signature_h[i + j] = 0;
			}
		}
	}
	if (EXIT_SUCCESS == contains_zero(signature_h, signature_block_size))
	{
		return EXIT_SUCCESS;
	}
	else
	{
		print_vector(signature_h, signature_block_size);
		return EXIT_FAILURE;
	}
}

int build_dyadic_signature_part2(gf *signature_h, int * block_position)
{
	int t, i, j; //for loop variables
	int ll = 0;
	int size_part = (F_q_m_size / signature_block_size) - 1;
	int part[(F_q_m_size / signature_block_size) - 1] = { 0 };
	int count_part = 0;
	gf aux_list[signature_block_size] = { 0 };
	gf temp_list[signature_block_size];// = { 0 };

	memcpy(temp_list, signature_h, signature_block_size * sizeof(gf));

	for (i = 0; i < size_part; i++) {
		part[i] = randombytes_uniform(size_part - 1);
	}

	//Check to see if temp_list does not contain zeros
		block_position[0] = ll;
		ll++;
		while (ll * signature_block_size != code_length) {
			j = 0;
			do {
				j = part[count_part];
				count_part++;
			} while (j == 0 && count_part < size_part);

			if (count_part >= size_part)
			{
				PRINT_DEBUG("Invalid index into part array\n");
				return EXIT_FAILURE;
			}

			if (EXIT_FAILURE == remove_integer(j, part, size_part))
			{
				PRINT_DEBUG("Failed to remove int likely already occurred\n");
			}

			memcpy(aux_list, &signature_h[(j * signature_block_size)], signature_block_size * sizeof(gf));

			if (EXIT_SUCCESS == contains_zero(aux_list, signature_block_size)) {
				block_position[ll] = j;
				ll++;
			}
		}
	return EXIT_SUCCESS;
}

static int build_dyadic_signature_part3(gf *signature_h, int * block_position, gf *dyadic_signature)
{
	int nr_blocks = F_q_m_size / signature_block_size;
	struct signature_block blocks[nr_blocks];
	int t, i, j; //for loop variables
	int aux_count_transfer_block = 0, block_nr = 0;

	for (i = 0; i < nr_blocks; i++) {
		if (NULL == (blocks[i].signature = (gf*) calloc(signature_block_size, sizeof(gf)))){
			PRINT_DEBUG("Failed to calloc space for block signatures\n");
			return EXIT_FAILURE;
		}
	}

	for (i = 0; i < F_q_m_size; i = i + signature_block_size) {
		for (j = 0; j < signature_block_size; j++) {
			blocks[block_nr].signature[j] = signature_h[i + j];
		}
		block_nr++;

	}
	gf trash_h[code_length];
	for (i = 0; i < n0; i++) {
		struct signature_block block = blocks[block_position[i]];
		for (int j = 0; j < signature_block_size; j++) {
			dyadic_signature[aux_count_transfer_block] = block.signature[j];
			aux_count_transfer_block++;
		}
		/*memcpy(&trash_h[aux_count_transfer_block], &blocks[i],
				signature_block_size * sizeof(gf));
		aux_count_transfer_block += signature_block_size;
		for( j =0; j < code_length; j++)
		{
			PRINT_DEBUG("%d:%d vs %d \n",j trash_h[i])
		}*/
	}


	for (int i = 0; i < nr_blocks; i++) {
		free(blocks[i].signature);
	}
	PRINT_DEBUG("Returning\n");
	return EXIT_SUCCESS;

}



int is_vectors_disjoint(gf *u, gf *v) {
	int i, j;
	PRINT_DEBUG("is_vector_disjoint\n");
	for (i = 0; i < (signature_block_size); i++) {
		for (j = 0; j < code_length; j++) {
			if (u[i] == v[j]) {
				return 1;
			}
		}
	}
	return 0;
}

int is_vector_disjoint(gf *list, int n) {
	int i, j;
	for (i = 0; i < n; i++) {
		for (j = i + 1; j < n; j++) {
			if (list[i] == list[j]) {
				return 1;
			}
		}
	}
	return 0;
}


/*
 * For loop through both arrays and if remove elements from the to_remove array
 * if they exist in they elements array.
 */
void remove_elements(gf *to_remove, gf *elements, int length) {
	int i, j;
	for (i = 0; i < length; i++) {
		for (j = 0; j < length; j++) {
			if (to_remove[i] == elements[j]) {
				to_remove[i] = 0;
			}

		}
	}
}

void build_support(gf *signature_h, gf *u, gf *v) {
	gf elements_in_F_q_m[F_q_m_size] = { 0 };
	gf aux[code_length] = { 0 };
	gf h0_inv = gf_q_m_inv(signature_h[0]);
	int i;
	gf omega = 0, sum_inv;

	generate_elements_in_F_q_m(elements_in_F_q_m);

	PRINT_DEBUG("build_support start\n");
	for (i = 0; i < code_length; i++) {
		aux[i] = h0_inv ^ gf_q_m_inv(signature_h[i]);
	}
	remove_elements(elements_in_F_q_m, aux, code_length);


	do {
		omega = elements_in_F_q_m[randombytes_uniform(code_length - 1)];
	} while (omega == 0);

	for (i = 0; i < code_length; i++) {
		if (signature_h[i] != 0) {
			v[i] = gf_q_m_inv(signature_h[i]) ^ h0_inv ^ omega;
		}
	}

	for (i = 0; i < signature_block_size; i++) {
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

	for (int k = 1; k < pol_deg + 1; k++) {
		for (int i = 0; i < signature_block_size; i++) {
			for (int j = 0; j < code_length; j++) {
				gf element = (H->data[i * H->cols + j]);
				gf result = gf_pow_f_q_m(element, k);
				H_cauchy->data[((k - 1) * signature_block_size + i) * H->cols
						+ j] = result;

			}
		}
	}
	free_matrix(H);
}

void build_trapdoor(const matrix *H_cauchy, const gf *v, const gf *u, gf *y,
		matrix *H) {
	PRINT_DEBUG("build_trapdoor start\n");
	gf z[code_length] = { 0 };

	for (int i = 0; i < n0; i++) {
		gf random_el = 0;
		do {
			random_el = randombytes_uniform(F_q_m_size - 1);
		} while (random_el == 0 || vector_contains(z, random_el, code_length));

		z[i * signature_block_size] = random_el;
		for (int j = 1; j < signature_block_size; j++) {
			z[(i * signature_block_size) + j] = z[i * signature_block_size];
		}
	}
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
			gf result = gf_pow_f_q_m(sum_u_v, pol_deg);
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

int generate_systematic_matrix(const matrix* Hbase) {

	int i, j, l = 0, test = 0;
	gf temp;
	int n = Hbase->cols;
	int k = Hbase->rows;
	for (i = 0; i < k; i++) {
		test = 0;
		l = 0;
		j = i + n - k;
		if (Hbase->data[(i * n) + i + n - k] == 0) { //We're looking for a non-zero pivot
			test = 1;
			//printf("search Pivot\n");
			for (l = i + 1; l < k; l++) {
				if (Hbase->data[l * n + j]) {
					//printf("Find Pivot\n");
					break;
				}
			}
		}
		if (l == k && (i != (k - 1))) {
			printf("Non systematic Matrix %d\n", l);
			return -1;
		}
		if (test == 1) { // We switches the lines l and i
			test = 0;
			//printf("Permut line\n");
			//temp=P[i+n-k];
			//P[i+n-k]=P[j];
			//P[j]=temp;
			for (j = 0; j < n; j++) {
				temp = Hbase->data[(l * Hbase->cols) + j];
				Hbase->data[(l * Hbase->cols) + j] = Hbase->data[(i
						* Hbase->cols) + j];
				Hbase->data[(i * Hbase->cols) + j] = temp;
			}
		}
		//   Matrix standardization
		gf invPiv = 1, aa;
		if (Hbase->data[(i * Hbase->cols) + i + n - k] != 1) {
			aa = Hbase->data[(i * Hbase->cols) + i + n - k];
			invPiv = gf_inv(aa);
			Hbase->data[(i * Hbase->cols) + i + n - k] = 1;

			for (j = 0; j < n; j++) {
				if (j == i + n - k) {
					continue;
				}
				Hbase->data[(i * Hbase->cols) + j] = gf_mult(
						Hbase->data[(i * Hbase->cols) + j], invPiv);
			}
		}

		//Here we do the elimination on column i + n-k
		gf piv_align;
		for (l = 0; l < k; l++) {
			if (l == i) {
				continue;
			}
			if (Hbase->data[(l * n) + i + n - k]) {
				piv_align = Hbase->data[(l * Hbase->cols) + i + n - k];

				for (j = 0; j < n; j++) {
					Hbase->data[(l * Hbase->cols) + j] = Hbase->data[(l
							* Hbase->cols) + j]
							^ (gf_mult(piv_align,
									Hbase->data[(i * Hbase->cols) + j]));
				}
			}
		}
	}
	return 0;


}

int generate_public_key(const matrix *Hbase, matrix *G) {

	int ret_val = generate_systematic_matrix(Hbase);
	if (ret_val) {
		return 1;
	}
	int n = Hbase->cols;
	int r = Hbase->rows;

	matrix *M = submatrix(Hbase, 0, 0, r, (n - r));

	//free_matrix(HH);

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
	return 0;

}

int key_pair_generation(unsigned char *pk, unsigned char *sk) {
	gf v[code_length] = { 0 };
	gf y[code_length] = { 0 };
	matrix G;
	G.cols = code_length;
	G.rows = code_length - ((signature_block_size * pol_deg) * extension);
	gf data_G[code_length
			* (code_length - ((signature_block_size * pol_deg) * extension))] =
			{ 0 };
	G.data = data_G;
	key_gen(v, y, &G);
	store(&G, pk);
	store_u_y(v, y, sk);

	return 0;
}



void key_gen(gf *v, gf *y, matrix *G) {
	int ret_value = 0;
	gf signature_h[code_length];
	gf u[signature_block_size];
	long build_support_failures = 0;
	long build_dyadic_sig_failures =0;

	PRINT_DEBUG("Key Gen start\n");

	do {
		memset(signature_h, 0, code_length * sizeof(gf));
		memset(u, 0, signature_block_size * sizeof(gf));
		do {
			build_support_failures ++;
			ret_value = build_dyadic_signature(signature_h);
			if (ret_value == EXIT_SUCCESS){
				build_support(signature_h, u, v);
			}
			else
			{
				build_dyadic_sig_failures ++;
				build_support_failures--;
			}
			PRINT_DEBUG("interations %ld vs %d\n",build_support_failures, build_dyadic_sig_failures);

		} while (ret_value != EXIT_SUCCESS || is_vectors_disjoint(u, v) || is_vector_disjoint(v, code_length)
				|| is_vector_disjoint(u, signature_block_size));

		//print_vector(signature_h, code_length);
		PRINT_DEBUG("Starting cauchy\n");
		matrix H_cauchy;
		H_cauchy.rows = signature_block_size * pol_deg;
		H_cauchy.cols = code_length;

		gf data_cauchy[signature_block_size * pol_deg * code_length] = { 0 };
		H_cauchy.data = data_cauchy;
		build_cauchy_matrix(u, v, &H_cauchy);

		matrix H;
		H.rows = signature_block_size * pol_deg;
		H.cols = code_length;
		gf data_H[signature_block_size * pol_deg * code_length] = { 0 };
		H.data = data_H;
		PRINT_DEBUG("Calling build_trapdoor\n");
		build_trapdoor(&H_cauchy, v, u, y, &H);

		matrix Hbase;
		Hbase.rows = (signature_block_size * pol_deg) * extension;
		Hbase.cols = code_length;
		gf data_Hbase[signature_block_size * pol_deg * code_length * extension] =
				{ 0 };
		Hbase.data = data_Hbase;
		PRINT_DEBUG("Calling project_H_on_F_q\n");
		project_H_on_F_q(&H, &Hbase);

		PRINT_DEBUG("Generating public_key\n");
		ret_value = generate_public_key(&Hbase, G);
		PRINT_DEBUG("generate_public_key returned %d\n",ret_value);

	} while (ret_value);
}
