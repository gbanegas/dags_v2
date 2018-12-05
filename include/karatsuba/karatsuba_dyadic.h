/*
 * karatsuba_dyadic.h
 *
 *  Created on: May 24, 2018
 *      Author: vader
 */

#ifndef SRC_KARATSUBA_DYADIC_H_
#define SRC_KARATSUBA_DYADIC_H_

#include <stdio.h>
#include <stdlib.h>

#include "param.h"
#include "definitions.h"
#include "gf.h"
#include "matrix_operations.h"
#include "matrix_utils.h"


inline void slicing_array(int *input, int start, int end, int *output) {
	int count = 0;
	for (int i = start; i < end; i++) {
		output[count] = input[i];
		count++;
	}
}

extern void karatsuba_k(int k, int * a, int *b, int *c_sig);

extern void karatsuba_gf(int k, gf * a, gf *b, gf *c_sig);

extern void karatsuba_n1_gf(int pos0, gf *a, gf*b );
extern void karatsuba_n2_gf(int pos0, gf *a, gf*b );
extern void karatsuba_n3_gf(int pos0, gf *a, gf*b );
extern void karatsuba_n4_gf(int pos0, gf *a, gf*b );
extern void karatsuba_n5_gf(int pos0, gf *a, gf*b );
extern void karatsuba_n6_gf(int pos0, gf *a, gf*b );
extern void karatsuba_n7_gf(int pos0, gf *a, gf*b );
extern void karatsuba_n8_gf(int pos0, gf *a, gf*b );
extern void karatsuba_n9_gf(int pos0, gf *a, gf*b );


void karatsuba_n_generic_gf(const int pos0, const int size, gf *a, gf*b );


#endif /* SRC_KARATSUBA_DYADIC_H_ */
