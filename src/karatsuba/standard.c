/*
 * standard.c
 *
 *  Created on: May 24, 2018
 *      Author: vader
 */

#include "standard.h"

void std_multiplication(int n, int *a, int *b, int *result) {
	int k = 1 << n;
	result[0] = a[0] * b[0];
	for (int i = 1; i < k; i++) {
		result[0] = result[0] + (a[i] * b[i]);
		for (int j = 0; j < k; j++) {
			int result_sum = i ^ j;
			result[i] = result[i] + (a[j] * b[result_sum]);
		}
	}

}

inline void std_multiplication_gf(int n, gf *a, gf *b, gf *result) {
	int k = 1 << n;
	result[0] = gf_mult_fast(a[0] , b[0]);
	for (int i = 1; i < k; i++) {
		result[0] = result[0]^ gf_mult_fast(a[i] , b[i]);
		for (int j = 0; j < k; j++) {
			int result_sum = i ^ j;
			result[i] = result[i] ^ gf_mult_fast(a[j] , b[result_sum]);
		}
	}

}
