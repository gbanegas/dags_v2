/*
 * encapsulation.c
 *
 *  Created on: May 4, 2018
 *      Author: vader
 */

#include "../include/encapsulation.h"

/*
 * encapsulation:
 *   This function produces the ciphert_text given the shared secret and
 *   public key
 *
 * Params:
 * 	cipher_text: Provide an array that can hold the size of the ciphertext
 * 	secret_shared: Provide the shared secret
 * 	public_key: Provide the public_key
 */
int encapsulation(unsigned char *ciphert_text, unsigned char *secret_shared,
		const unsigned char *public_key) {
	int result;
	matrix *G = NULL;

	PRINT_DEBUG_ENCAP("Creating Matrix G: \n");
	G = make_matrix(code_dimension, code_length);
	PRINT_DEBUG_ENCAP("Recovering Matrix G: \n");
	recover_public_key(public_key, G);
	print_matrix(G);

	result = encrypt(ciphert_text, secret_shared, G);
	free_matrix(G);
	G = NULL;
	return result;
}

int encrypt(unsigned char *ciphert_text, unsigned char *secret_shared,
		matrix *G) {

	int i;
	unsigned char m[code_length] = { 0 }; //[k_prime] = { 0 };
	unsigned char d[k_prime] = { 0 };
	unsigned char rho[k_sec] = { 0 };
	unsigned char error_array[code_length] = { 0 };
	unsigned char sigma[(code_dimension - k_sec)] = { 0 };
	unsigned char hash_sigma[code_length] = { 0 };

	unsigned char u[code_dimension] = { 0 };
	gf c[code_length] = { 0 };
	unsigned char r[code_dimension] = { 0 };
	unsigned char dd[k_prime] = { 0 };
	unsigned char K[ss_length] = { 0 };

	PRINT_DEBUG_ENCAP("Generation Random M: \n");
	random_m(m);
	for (i = 0; i < k_prime; i++) {
		m[i] = m[i] % F_q_size;
	}

#ifdef DEBUG_ENCAP

	for (i = 0; i < k_prime; i++) {
		PRINT_DEBUG_ENCAP(" %" PRIu16 ", ", m[i]);
	}
	PRINT_DEBUG_ENCAP("\nStarting hashing: \n");
#endif
	int test = KangarooTwelve(m, k_prime, r, code_dimension, K12_custom, K12_custom_len);
	assert(test == 0); // Catch Error

	// m: input type unsigned char len k_prime | d: output type unsigned char len k_prime
	test = KangarooTwelve(m, k_prime, d, k_prime, K12_custom, K12_custom_len);
	assert(test == 0); // Catch Error

	// Type conversion
	for (i = 0; i < k_prime; i++)
		// Optimize modulo
		dd[i] = (unsigned char) (d[i] % F_q_size);
	/*SHAKE256(r, code_dimension, m, k_prime);
	 SHAKE256(d, k_prime, m, k_prime);

	 for (i = 0; i < k_prime; i++) {
	 // Optimize modulo
	 dd[i] = (unsigned char) (d[i] & F_q_order);
	 }*/

	PRINT_DEBUG_ENCAP("Generating sigma and rho: \n");
	for (i = 0; i < code_dimension; i++) {
		if (i < k_sec) {
			// Optimize modulo

			rho[i] = (unsigned char) (r[i] % F_q_size); //rho recovery
		} else {
			// Optimize modulo

			sigma[i - k_sec] = (unsigned char) (r[i] % F_q_size); // sigma recovery
		}
	}

	PRINT_DEBUG_ENCAP("Expanding m: \n");

	for (i = 0; i < code_dimension; i++) {
		if (i < k_sec) {
			u[i] = ((unsigned char) rho[i]);
		} else {
			u[i] = ((unsigned char) m[i - k_sec]);
		}
	}

	PRINT_DEBUG_ENCAP("Generating error_array: \n");
	/*SHAKE256(hash_sigma, code_length, sigma, k_prime);*/
	test = KangarooTwelve(sigma, k_prime, hash_sigma, code_length, K12_custom,
	K12_custom_len);
	assert(test == 0); // Catch Error
	random_e(hash_sigma, error_array);

#ifdef DEBUG_ENCAP
	PRINT_DEBUG_ENCAP("message:\n");
	for (i = 0; i < code_dimension - k_sec; i++)
		PRINT_DEBUG_ENCAP(" %" PRIu16 ", ", u[i]);
	PRINT_DEBUG_ENCAP("\nEncaps_error_array:\n");
	for (i = 0; i < code_length; i++) {
		printf(" %" PRIu16 ", ", error_array[i]);
	}
	PRINT_DEBUG_ENCAP("\n");
#endif

	PRINT_DEBUG_ENCAP("Computing m*G: \n");
	multiply_vector_matrix(u, G, c);//c = message*G

	PRINT_DEBUG_ENCAP("Computing (m*G) + error: \n");
	for (i = 0; i < code_length + k_prime; i++) {
		if (i < code_length) {
			ciphert_text[i] = (c[i] ^ error_array[i]); //c + error
		} else {
			ciphert_text[i] = dd[i - code_length];
		}
	}

#ifdef DEBUG_ENCAP
	for (i = 0; i < code_length; i++)
		PRINT_DEBUG_ENCAP(" %" PRIu16 ", ", ciphert_text[i]);
	PRINT_DEBUG_ENCAP("|\nHashing (m*G) + error: \n");
#endif
	//SHAKE256(K, ss_length, m, k_prime);
	test = KangarooTwelve(m, k_prime, K, ss_length, K12_custom, K12_custom_len);
	assert(test == 0); // Catch Error
	for (i = 0; i < ss_length; i++) {
		secret_shared[i] = K[i];
	}
	/*free(u);
	 free(m);*/
	return 0;
}
