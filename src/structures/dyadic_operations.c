/*
 * dyadic_inverse.c
 *
 *  Created on: May 8, 2018
 *      Author: vader
 */

#include "../../include/structures/dyadic_operations.h"

void dyadic_inverse(int n, gf *signature, gf* signature_inverse) {
	int i;
	gf * sig_inv = calloc(n, sizeof(gf));
	gf det = 0;

	for (i = 0; i < n; i++) {
		det = gf_sum(det, signature[i]);
	}
	det = gf_mult(det, det);
	gf det_inv = gf_inv(det);
	for (i = 0; i < n; i++) {
		sig_inv[i] = gf_mult(det_inv, signature[i]);
	}

	for (i = 0; i < n; i++) {
		signature_inverse[i] = sig_inv[i];
	}
	free(sig_inv);
}

void dyadic_sum(int n, gf *signature_a, gf *signature_b, gf *signature_result) {
	int i;
	for (i = 0; i < n; i++) {
		signature_result[i] = gf_sum(signature_a[i], signature_b[i]);
	}
}



void dyadic_product(int n, gf *signature_a, gf *signature_b,
		gf *signature_result) {

	//karatsuba_gf((1<<n), signature_a, signature_b, signature_result);

	std_multiplication_gf(n, signature_a, signature_b, signature_result);

	//multiply_dyadic_convolution_gf(n, signature_a, signature_b, signature_result);

	/*int i, j;

	for (i = 0; i < n; i++) {
		gf current_product = 0;
		for (j = 0; j < n; j++) {
			int dec_position = i^j;
			gf sum_term = gf_mult(signature_a[j],
					signature_b[dec_position]);
			current_product = gf_sum(current_product, sum_term);
		}
		signature_result[i] = current_product;
	}*/

}
