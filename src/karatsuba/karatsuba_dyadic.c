/*
 * karatsuba_dyadic.c
 *
 *  Created on: May 24, 2018
 *      Author: vader
 */

#include "../../karatsuba/karatsuba_dyadic.h"

inline void karatsuba_k(int k, int * a, int *b, int *c_sig) {
	int i;
	if (k == 2) {
		c_sig[0] = a[0] * b[0] + a[1] * b[1];
		c_sig[1] = a[0] * b[1] + a[1] * b[0];
	} else {
		int k_2 = k / 2;
		//First half of the result
		int c_sig_sliced_d[k_2];
		int a_sliced_d[k_2];
		int b_sliced_d[k_2];

		//slicing_array(c_sig, 0, k_2, c_sig_sliced_d);
		for (int i = 0; i < k_2; i++) {
			c_sig_sliced_d[i] = c_sig[i];
			a_sliced_d[i] = a[i];
			b_sliced_d[i] = b[i];
		}

		karatsuba_k(k_2, a_sliced_d, b_sliced_d, c_sig_sliced_d);

		int c_sig_sliced_u[k - k_2];
		int a_sliced_u[k - k_2];
		int b_sliced_u[k - k_2];

		for (i = k_2; i < k; i++) {
			c_sig_sliced_u[i - k_2] = c_sig[i];
			a_sliced_u[i - k_2] = a[i];
			b_sliced_u[i - k_2] = b[i];
		}
		//	slicing_array(c_sig, k_2, k, c_sig_sliced_u);
		//	slicing_array(a, k_2, k, a_sliced_u);
		//	slicing_array(b, k_2, k, b_sliced_u);

		karatsuba_k(k_2, a_sliced_u, b_sliced_u, c_sig_sliced_u);
		for (i = k_2; i--;) {
			c_sig[i] = c_sig_sliced_d[i] + c_sig_sliced_u[i];
		}

		int a_plus_a_sliced[k_2];
		int b_plus_b_sliced[k_2];

		for (i = k_2; i--;) {
			a_plus_a_sliced[i] = a[i] + a[i + k_2];
			b_plus_b_sliced[i] = b[i] + b[i + k_2];
		}

		int c_sig_result_sum[k_2];

		karatsuba_k(k_2, a_plus_a_sliced, b_plus_b_sliced, c_sig_result_sum);

		for (int i = k_2; i < k; i++) {
			c_sig[i] = c_sig_result_sum[i - k_2];
			c_sig[i] = c_sig[i] - c_sig[i - k_2];
		}
	}

}

inline void karatsuba_gf(int k, gf * a, gf *b, gf *c_sig) {
	int i;
	if (k <= 2) {
		c_sig[0] = gf_mult_fast(a[0] , b[0]) ^ gf_mult_fast(a[1] , b[1]);
		c_sig[1] = gf_mult_fast(a[0] , b[1]) ^ gf_mult_fast(a[1] , b[0]);
	} else {
		int k_2 = k / 2;
		//First half of the result
		gf c_sig_sliced_d[k_2];
		gf a_sliced_d[k_2];
		gf b_sliced_d[k_2];

		//slicing_array(c_sig, 0, k_2, c_sig_sliced_d);
		for (int i = 0; i < k_2; i++) {
			c_sig_sliced_d[i] = c_sig[i];
			a_sliced_d[i] = a[i];
			b_sliced_d[i] = b[i];
		}

		karatsuba_gf(k_2, a_sliced_d, b_sliced_d, c_sig_sliced_d);

		gf c_sig_sliced_u[k - k_2];
		gf a_sliced_u[k - k_2];
		gf b_sliced_u[k - k_2];

		for (i = k_2; i < k; i++) {
			c_sig_sliced_u[i - k_2] = c_sig[i];
			a_sliced_u[i - k_2] = a[i];
			b_sliced_u[i - k_2] = b[i];
		}
		//	slicing_array(c_sig, k_2, k, c_sig_sliced_u);
		//	slicing_array(a, k_2, k, a_sliced_u);
		//	slicing_array(b, k_2, k, b_sliced_u);

		karatsuba_gf(k_2, a_sliced_u, b_sliced_u, c_sig_sliced_u);
		for (i = k_2; i--;) {
			c_sig[i] = c_sig_sliced_d[i] ^ c_sig_sliced_u[i];
		}

		gf a_plus_a_sliced[k_2];
		gf b_plus_b_sliced[k_2];

		for (i = k_2; i--;) {
			a_plus_a_sliced[i] = a[i] ^a[i + k_2];
			b_plus_b_sliced[i] = b[i] ^ b[i + k_2];
		}

		gf c_sig_result_sum[k_2];

		karatsuba_gf(k_2, a_plus_a_sliced, b_plus_b_sliced, c_sig_result_sum);

		for (int i = k_2; i < k; i++) {
			c_sig[i] = c_sig_result_sum[i - k_2];
			c_sig[i] = c_sig[i]^c_sig[i - k_2];
		}
	}

}

void karatsuba_n1_gf(int pos0, gf *a, gf*b ){
	int pos1=pos0+1;
	gf tmp_a=a[pos0]^a[pos1];
	gf tmp_b=b[pos0]^b[pos1];
	a[pos0]=gf_mult_fast(a[pos0],b[pos0])^gf_mult_fast(a[pos1],b[pos1]);
	a[pos1]=a[pos0]^gf_mult_fast(tmp_a,tmp_b);
}

void karatsuba_n2_gf(int pos0, gf *a, gf*b ){
	gf tmp_a[2];
	gf tmp_b[2];
	int pos2=pos0+2;
	for(int i=0;i<2;i++){
		tmp_a[i]=a[pos0+i]^a[pos2+i];
		tmp_b[i]=b[pos0+i]^b[pos2+i];
	}

	karatsuba_n1_gf(pos0,a,b);
	karatsuba_n1_gf(pos2,a,b);
	karatsuba_n1_gf(0,tmp_a,tmp_b);
	for(int i=0;i<2;i++){
		a[pos0+i]=a[pos0+i]^a[pos2+i];
		a[pos2+i]=a[pos0+i]^tmp_a[i];
	}
}

void karatsuba_n3_gf(int pos0, gf *a, gf*b ){
	gf tmp_a[4];
	gf tmp_b[4];
	int pos4=pos0+4;
	for(int i=0;i<4;i++){
		tmp_a[i]=a[pos0+i]^a[pos4+i];
		tmp_b[i]=b[pos0+i]^b[pos4+i];
	}

	karatsuba_n2_gf(pos0,a,b);
	karatsuba_n2_gf(pos4,a,b);
	karatsuba_n2_gf(0,tmp_a,tmp_b);
	for(int i=0;i<4;i++){
		a[pos0+i]=a[pos0+i]^a[pos4+i];
		a[pos4+i]=a[pos0+i]^tmp_a[i];
	}
}

void karatsuba_n4_gf(int pos0, gf *a, gf*b ){
	gf tmp_a[8];
	gf tmp_b[8];
	int pos8=pos0+8;
	for(int i=0;i<8;i++){
		tmp_a[i]=a[pos0+i]^a[pos8+i];
		tmp_b[i]=b[pos0+i]^b[pos8+i];
	}

	karatsuba_n3_gf(pos0,a,b);
	karatsuba_n3_gf(pos8,a,b);
	karatsuba_n3_gf(0,tmp_a,tmp_b);
	for(int i=0;i<8;i++){
		a[pos0+i]=a[pos0+i]^a[pos8+i];
		a[pos8+i]=a[pos0+i]^tmp_a[i];
	}
}


void karatsuba_n5_gf(int pos0, gf *a, gf*b ){
	gf tmp_a[16];
	gf tmp_b[16];
	int pos16=pos0+16;
	for(int i=0;i<16;i++){
		tmp_a[i]=a[pos0+i]^a[pos16+i];
		tmp_b[i]=b[pos0+i]^b[pos16+i];
	}

	karatsuba_n4_gf(pos0,a,b);
	karatsuba_n4_gf(pos16,a,b);
	karatsuba_n4_gf(0,tmp_a,tmp_b);
	for(int i=0;i<16;i++){
		a[pos0+i]=a[pos0+i]^a[pos16+i];
		a[pos16+i]=a[pos0+i]^tmp_a[i];
	}
}
void karatsuba_n6_gf(int pos0, gf *a, gf*b ){
	gf tmp_a[32];
	gf tmp_b[32];
	int pos32=pos0+32;
	for(int i=0;i<32;i++){
		tmp_a[i]=a[pos0+i]^a[pos32+i];
		tmp_b[i]=b[pos0+i]^b[pos32+i];
	}

	karatsuba_n5_gf(pos0,a,b);
	karatsuba_n5_gf(pos32,a,b);
	karatsuba_n5_gf(0,tmp_a,tmp_b);
	for(int i=0;i<32;i++){
		a[pos0+i]=a[pos0+i]^a[pos32+i];
		a[pos32+i]=a[pos0+i]^tmp_a[i];
	}
}

void karatsuba_n7_gf(int pos0, gf *a, gf*b ){
	gf tmp_a[64];
	gf tmp_b[64];
	int pos64=pos0+64;
	for(int i=0;i<64;i++){
		tmp_a[i]=a[pos0+i]^a[pos64+i];
		tmp_b[i]=b[pos0+i]^b[pos64+i];
	}

	karatsuba_n6_gf(pos0,a,b);
	karatsuba_n6_gf(pos64,a,b);
	karatsuba_n6_gf(0,tmp_a,tmp_b);
	for(int i=0;i<64;i++){
		a[pos0+i]=a[pos0+i]^a[pos64+i];
		a[pos64+i]=a[pos0+i]^tmp_a[i];
	}
}

void karatsuba_n8_gf(int pos0, gf *a, gf*b ){
	gf tmp_a[128];
	gf tmp_b[128];
	int pos64=pos0+128;
	for(int i=0;i<128;i++){
		tmp_a[i]=a[pos0+i]^a[pos64+i];
		tmp_b[i]=b[pos0+i]^b[pos64+i];
	}

	karatsuba_n7_gf(pos0,a,b);
	karatsuba_n7_gf(pos64,a,b);
	karatsuba_n7_gf(0,tmp_a,tmp_b);
	for(int i=0;i<128;i++){
		a[pos0+i]=a[pos0+i]^a[pos64+i];
		a[pos64+i]=a[pos0+i]^tmp_a[i];
	}
}

void karatsuba_n9_gf(int pos0, gf *a, gf*b ){
	gf tmp_a[256];
	gf tmp_b[256];
	int pos64=pos0+256;
	for(int i=0;i<256;i++){
		tmp_a[i]=a[pos0+i]^a[pos64+i];
		tmp_b[i]=b[pos0+i]^b[pos64+i];
	}

	karatsuba_n8_gf(pos0,a,b);
	karatsuba_n8_gf(pos64,a,b);
	karatsuba_n8_gf(0,tmp_a,tmp_b);
	for(int i=0;i<256;i++){
		a[pos0+i]=a[pos0+i]^a[pos64+i];
		a[pos64+i]=a[pos0+i]^tmp_a[i];
	}
}


void karatsuba_n_generic_gf(const int pos0, const int size, gf *a, gf*b ){
	if (size == 0)
		return;
	gf tmp_a[size];
	gf tmp_b[size];
	int pos64=pos0+size;
	for(int i=0;i<size;i++){
		tmp_a[i]=a[pos0+i]^a[pos64+i];
		tmp_b[i]=b[pos0+i]^b[pos64+i];
	}

	karatsuba_n_generic_gf(pos0,size/2,a,b);
	karatsuba_n_generic_gf(pos64,size/2,a,b);
	karatsuba_n_generic_gf(0,size/2,tmp_a,tmp_b);
	for(int i=0;i<size;i++){
		a[pos0+i]=a[pos0+i]^a[pos64+i];
		a[pos64+i]=a[pos0+i]^tmp_a[i];
	}
}

