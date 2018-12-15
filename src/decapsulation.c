/*
 * decapsulation.c
 *
 *  Created on: May 4, 2018
 *      Author: vader
 */

#include "../include/decapsulation.h"

/*
 * Decapsulation:
 *   This function can be used to decrypt the cipher_text.
 *   The secret_key is used to obtain both v and y for the decrypt function
 *
 * Param:
 *   secret_shard: Provide the shared secret
 *   cipher_text:  Provide the cipher text
 *   secret_key:   Provide the secret key
 * Returns: the results from decrypt() based on provided input
 */
int decapsulation(unsigned char *secret_shared,
		const unsigned char *cipher_text, const unsigned char *secret_key) {
	gf v[code_length] = { 0 };
	gf y[code_length] = { 0 };
	set_vy_from_sk(v, y, secret_key);
	return decrypt(secret_shared, cipher_text, v, y);
}

int decrypt(unsigned char *secret_shared, const unsigned char *cipher_text,
		const gf *v, const gf *y) {
	int i, decode_value, check_return;
	unsigned char word[code_length] = { 0 };
	unsigned char m1[k_prime] = { 0 };
	unsigned char rho1[k_sec] = { 0 };
	unsigned char r1[code_dimension] = { 0 };
	unsigned char d1[k_prime] = { 0 };
	unsigned char rho2[k_sec] = { 0 };
	unsigned char sigma[code_dimension] = { 0 };
	unsigned char e2[code_length] = { 0 };
	unsigned char hash_sigma[code_length] = { 0 };
	unsigned char e_prime[code_length] = { 0 };

	/*
	 * Step_1 of the decapsulation :  Decode the noisy codeword C received as
	 * part of the ciphertext ct = (c||d) with d is “ a plaintext confirmation”.
	 * We obtain codeword mot = u1G and error e
	 */

	PRINT_DEBUG_DECAP("Starting decoding...\n");
	decode_value = decoding(v, y, cipher_text, e_prime, word);

	/*
	 * Step_2 of the decapsulation :  Output ⊥ if decoding fails or wt(e) != n0_w
	 */

	if (decode_value == EXIT_FAILURE || compute_weight(e_prime, code_length) != weight) {
		return -1;
	}

	/*
	 * Step_3 of the decapsulation :  Recover u_prime = word and parse it as (rho1||m1)
	 */
	// Optimize modulo and removed copy to u1
	memcpy(rho1, word, k_sec);
	memcpy(m1, word + k_sec, code_dimension - k_sec);

	/*
	 * Step_4 of the decapsulation :  Compute r1 = G(m1) and d1 = H(m1)
	 */

	
	shake128(r1, code_dimension, m1, k_prime);
	
	// Compute d1 = H(m1) where H is  sponge SHA-512 function

	shake128(d1, k_prime, m1, k_prime);
	
	for (i = 0; i < k_prime; i++) {
		d1[i] = d1[i] & (F_q_size - 1);
	}
	// Return -1 if d distinct d1.
	// d starts at ct+code_length.
	if (memcmp(cipher_text + code_length, d1, k_prime) != 0) {
		return EXIT_FAILURE;
	}

	/*
	 * Step_5 of the decapsulation: Parse r1 as (rho2||sigma1)
	 */
	for (i = 0; i < code_dimension; i++) {
		if (i < k_sec) {
			// Optimize modulo
			rho2[i] = r1[i] & (F_q_size - 1); //rho2 recovery
		} else {
			// Optimize modulo
			sigma[i - k_sec] = r1[i] & (F_q_size - 1); // sigma1 recovery
		}
	}

	//Return ⊥ if rho1 distinct rho2
	if (memcmp(rho1, rho2, k_sec) != 0) {
		return -1;
	}

	/*
	 * Step_6 of the decapsulation: Generate error vector e2 of length n and
	 * weight n0_w from sigma1
	 */

	//test = KangarooTwelve(sigma, k_prime, hash_sigma, code_length, K12_custom, K12_custom_len);
	//SHAKE256(hash_sigma, code_length, sigma, k_prime);
	shake128(hash_sigma, code_length, sigma, k_prime);
	
	//Generate error vector e2 of length code_length and weight n0_w from
	//hash_sigma1 by using random_e function.
	random_e(hash_sigma, e2);

	/*
	 * Step_7 of the decapsulation: Return ⊥ if e_prime distinct e.
	 */
	if (memcmp(e_prime, e2, code_length) != 0) {
		return -1;
	}

	/*
	 * Step_7 of the decapsulation: If the previous condition is not satisfied,
	 * compute the shared secret ss by using KangarooTwelve
	 */
	//test = KangarooTwelve(m1, k_prime, ss, ss_length, K12_custom, K12_custom_len);
	//SHAKE256(secret_shared, ss_length, m1, k_prime);
	shake128(secret_shared, ss_length, m1, k_prime);
	
	return 0;
}
