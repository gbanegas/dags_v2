/*
 * encapsulation.c
 *
 *  Created on: May 4, 2018
 *      Author: vader
 */

#include "encapsulation.h"

int encapsulation(unsigned char *ciphert_text, unsigned char *secret_shared,
		const unsigned char *public_key) {

#ifdef DEBUG_P
	printf("Creating Matrix G: \n");
#endif
	matrix *G = make_matrix(code_dimension, code_length);
#ifdef DEBUG_P
	printf("Recovering Matrix G: \n");
#endif
	recover_public_key(public_key, G);
	print_matrix(G);

	return encrypt(ciphert_text, secret_shared, G);
}

int encrypt(unsigned char *ciphert_text, unsigned char *secret_shared,
		matrix *G) {

	int i;
	unsigned char *m; //[k_prime] = { 0 };
	m = calloc(code_length, sizeof(unsigned char));
	unsigned char d[k_prime] = { 0 };
	unsigned char rho[k_sec] = { 0 };
	unsigned char error_array[code_length] = { 0 };
	unsigned char sigma[(code_dimension - k_sec)] = { 0 };
	unsigned char hash_sigma[code_length] = { 0 };

	unsigned char *u = calloc(code_dimension + 1, sizeof(unsigned char));
	gf c[code_length] = { 0 };
	unsigned char r[code_dimension] = { 0 };
	unsigned char dd[k_prime] = { 0 };
	unsigned char K[ss_length] = { 0 };

#ifdef DEBUG_P
	printf("Generation Random M: \n");
#endif
	random_m(m);

#ifdef DEBUG_P
	for(int i = 0; i < k_sec; i++){
		printf(" %" PRIu16 ", ", m[i]);
	}
	printf("Starting hashing: \n");
#endif
	SHAKE256(r, code_dimension, m, k_prime);
	SHAKE256(d, k_prime, m, k_prime);

	for (i = 0; i < k_prime; i++) {
		// Optimize modulo
		dd[i] = (unsigned char) (d[i] & F_q_order);
	}

#ifdef DEBUG_P
	printf("Generating sigma and rho: \n");
#endif
	for (i = 0; i < code_dimension; i++) {
		if (i < k_sec) {
			// Optimize modulo

			rho[i] = (unsigned char) (r[i] & F_q_order); //rho recovery
		} else {
			// Optimize modulo

			sigma[i - k_sec] = (unsigned char) (r[i] & F_q_order); // sigma recovery
		}
	}

#ifdef DEBUG_P
	printf("Expanding m: \n");
#endif

	for (i = 0; i < code_dimension; i++) {
		if (i < k_sec) {
			u[i] = ((unsigned char) rho[i]);
		} else {
			u[i] = ((unsigned char) m[i - k_sec]);
		}
	}


#ifdef DEBUG_P
	printf("Generating error_array: \n");
#endif
	SHAKE256(hash_sigma, code_length, sigma, k_prime);
	random_e(hash_sigma, error_array);

#ifdef DEBUG_P
	printf("message:\n");
	for (i = 0; i < code_dimension-k_sec; i++)
		printf(" %" PRIu16 ", ", u[i]);
	printf("\n");
	printf("encaps_error_array:\n");
	for (i = 0; i < code_length; i++) {
		printf(" %" PRIu16 ", ", error_array[i]);
	}
	printf("\n");
#endif

#ifdef DEBUG_P
	//printf("Computing m*G: \n");
#endif
	multiply_vector_matrix(u, G, c);

#ifdef DEBUG_P
	//printf("Computing (m*G) + error: \n");
#endif
	for (i = 0; i < code_length + k_prime; i++) {
		if (i < code_length) {
			ciphert_text[i] = (((unsigned char) c[i]) ^ error_array[i]); //message*G + error
		} else {
			ciphert_text[i] = dd[i - code_length];
		}
	}

#ifdef DEBUG_P
	for (i = 0; i < code_length; i++)
		printf(" %" PRIu16 ", ", ciphert_text[i]);
	printf("|\n");
	printf("hashing (m*G) + error: \n");
#endif
	SHAKE256(K, ss_length, m, k_prime);
	for (i = 0; i < ss_length; i++) {
		secret_shared[i] = K[i];
	}
	free(u);
	free(m);
	return 0;
}
