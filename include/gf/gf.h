/*
 * gf.h
 *
 *  Created on: Dec 1, 2017
 *      Author: vader
 */

#ifndef GF_H_
#define GF_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>

#include "definitions.h"
#include "../parameters/param.h"

static gf gf_mult(gf in0, gf in1);
static inline gf gf_q_m_mult(gf in0, gf in1);
static inline gf relative_field_representation(gf a, int k);

extern gf gf_q_m_inv(gf in);
extern gf gf_pow_f_q_m(gf in, gf power);
extern gf gf_pow_f_q(gf in, gf power);

static inline gf absolut_field_representation(gf* element);

extern gf gf_inv(gf in);

extern gf gf_div(gf a, gf b);

extern gf gf_div_f_q_m(gf a, gf b);

extern void print_F_q_element(gf a);

extern void print_F_q_m_element(gf a);

#if defined(DAGS_3) || defined(DAGS_5) || defined(DAGS_TOY)
static inline gf gf_mult(gf in0, gf in1) {
	uint64_t i, tmp, t0 = in0, t1 = in1;

	//Multiplication
	tmp = t0 * (t1 & 1);

	for (i = 1; i < 10; i++)
	tmp ^= (t0 * (t1 & (1 << i)));

	//reduction
	tmp = tmp & 0x7FFF;// tmp & 0111 1111 1111 1111
	//first step of reduction
	gf reduction = (tmp >> 8);
	tmp = tmp & 0xFF;
	tmp = tmp ^ (reduction << 4);
	tmp = tmp ^ (reduction << 3);
	tmp = tmp ^ reduction << 2;
	tmp = tmp ^ reduction;
	//second step of reduction
	reduction = (tmp >> 8);
	tmp = tmp ^ (reduction << 4);
	tmp = tmp ^ (reduction << 3);

	tmp = tmp ^ reduction << 2;
	tmp = tmp ^ reduction;

	tmp = tmp & 0xFF;
	return tmp;
}
//todo: finish here
static inline gf gf_q_m_mult(gf in0, gf in1) {
	uint64_t i, tmp, t0 = in0, t1 = in1;

	//Multiplication
	tmp = t0 * (t1 & 1);

	for (i = 1; i < 18; i++)
	tmp ^= (t0 * (t1 & (1 << i)));

	//reduction
	tmp = tmp & 0x7FFFFFFF;// tmp & 0111 1111 1111 1111
	//first step of reduction
	gf reduction = (tmp >> 16);
	tmp = tmp & 0xFFFF;
	tmp = tmp ^ (reduction << 5);
	tmp = tmp ^ (reduction << 3);
	tmp = tmp ^ reduction << 2;
	tmp = tmp ^ reduction;
	//second step of reduction
	reduction = (tmp >> 16);
	tmp = tmp ^ (reduction << 5);
	tmp = tmp ^ (reduction << 3);
	tmp = tmp ^ reduction << 2;
	tmp = tmp ^ reduction;

	tmp = tmp & 0xFFFF;
	return tmp;
}

static inline gf relative_field_representation(gf a, int k) {
	gf x[extension] = {0};
	gf b_0_t = a & 0x1;
	gf b_1_t = (a & 0x2) >> 1;
	gf b_2_t = (a & 0x4) >> 2;
	gf b_3_t = (a & 0x8) >> 3;
	gf b_4_t = (a & 0x10) >> 4;
	gf b_5_t = (a & 0x20) >> 5;
	gf b_6_t = (a & 0x40) >> 6;
	gf b_7_t = (a & 0x80) >> 7;
	gf b_8_t = (a & 0x100) >> 8;
	gf b_9_t = (a & 0x200) >> 9;
	gf b_10_t = (a & 0x400) >> 10;
	gf b_11_t = (a & 0x800) >> 11;
	gf b_12_t = (a & 0x1000) >> 12;
	gf b_13_t = (a & 0x2000) >> 13;
	gf b_14_t = (a & 0x4000) >> 14;
	gf b_15_t = (a & 0x8000) >> 15;

	gf b_0 = b_0_t ^ b_8_t ^ b_9_t ^ b_10_t ^ b_11_t ^ b_12_t;

	gf b_1 = b_2_t ^ b_3_t ^ b_4_t ^ b_5_t ^ b_8_t ^ b_11_t ^ b_13_t ^ b_14_t;

	gf b_2 = b_4_t ^ b_6_t ^ b_8_t ^ b_11_t ^ b_13_t ^ b_14_t;

	gf b_3 = b_3_t ^ b_5_t ^ b_8_t ^ b_9_t ^ b_10_t ^ b_11_t ^ b_15_t;

	gf b_4 = b_6_t ^ b_7_t ^ b_8_t ^ b_9_t ^ b_10_t;

	gf b_5 = b_4_t ^ b_5_t ^ b_6_t ^ b_7_t ^ b_8_t ^ b_10_t ^ b_11_t ^ b_12_t
	^ b_13_t ^ b_15_t;

	gf b_6 = b_6_t ^ b_7_t ^ b_8_t ^ b_9_t ^ b_10_t ^ b_12_t ^ b_14_t;

	gf b_7 = b_5_t ^ b_7_t ^ b_8_t ^ b_9_t ^ b_13_t ^ b_14_t ^ b_15_t;

	gf b_8 = b_1_t ^ b_2_t ^ b_3_t ^ b_4_t ^ b_7_t ^ b_10_t ^ b_12_t ^ b_13_t
	^ b_15_t;

	gf b_9 = b_3_t ^ b_5_t ^ b_8_t ^ b_9_t ^ b_11_t ^ b_12_t ^ b_13_t ^ b_15_t;

	gf b_10 = b_2_t ^ b_4_t ^ b_11_t ^ b_14_t ^ b_15_t;

	gf b_11 = b_5_t ^ b_6_t ^ b_10_t ^ b_11_t ^ b_15_t;

	gf b_12 = b_3_t ^ b_4_t ^ b_5_t ^ b_6_t ^ b_7_t ^ b_9_t ^ b_10_t ^ b_11_t
	^ b_12_t ^ b_14_t ^ b_15_t;

	gf b_13 = b_5_t ^ b_6_t ^ b_7_t ^ b_8_t ^ b_9_t ^ b_11_t ^ b_13_t;

	gf b_14 = b_4_t ^ b_6_t ^ b_7_t ^ b_8_t ^ b_12_t ^ b_13_t ^ b_14_t ^ b_15_t;

	gf b_15 = b_7_t ^ b_8_t ^ b_9_t ^ b_10_t ^ b_11_t ^ b_15_t;

	x[0] = (b_0) | (b_1 << 1) | (b_2 << 2) | (b_3 << 3) | (b_4 << 4)
	| (b_5 << 5) | (b_6 << 6) | (b_7 << 7);
	x[1] = (b_8) | (b_9 << 1) | (b_10 << 2) | (b_11 << 3) | (b_12 << 4)
	| (b_13 << 5) | (b_14 << 6) | (b_15 << 7);
	return x[k];
}

static inline gf absolut_field_representation(gf *element) {
	gf beta = 788;
	gf tmp1 = 0, tmp2 = 0, in0 = element[0], in1 = element[1];

	for (int i = 0; i < 8; i++) {
		gf power = gf_pow_f_q_m(beta, i);
		if (in0 & (1 << i))
		tmp1 ^= power;
	}
	for (int i = 0; i < 8; i++) {
		gf power = gf_pow_f_q_m(beta, i);
		if (in1 & (1 << i))
		tmp2 ^= power;
	}

	gf tmp_3 = gf_q_m_mult(1, tmp1);
	gf tmp_4 = gf_q_m_mult(2, tmp2);

	gf result = tmp_3 ^ tmp_4;
	return result;

}
#endif

#ifdef DAGS_1
static inline gf gf_mult(gf in0, gf in1) {
	uint64_t i, tmp, t0 = in0, t1 = in1;

	//Multiplication
	tmp = t0 * (t1 & 1);

	for (i = 1; i < 7; i++)
		tmp ^= (t0 * (t1 & (1 << i)));

	//reduction
	tmp = tmp & 0xFFF;	// tmp & 0000 1111 1111 1111
	tmp = tmp ^ (tmp >> 6);
	tmp = tmp ^ ((tmp >> 5) & 0x3E);
	tmp = tmp & 0x3F;
	return tmp;
}

static inline gf gf_q_m_mult(gf in0, gf in1) {
	uint64_t i, tmp, t0 = in0, t1 = in1;

	//Multiplication
	tmp = t0 * (t1 & 1);

	for (i = 1; i < 18; i++)
		tmp ^= (t0 * (t1 & (1 << i)));

	//reduction
	tmp = tmp & 0x7FFFFF;	// tmp & 0111 1111 1111 1111
	//first step of reduction
	gf reduction = (tmp >> 12);
	tmp = tmp & 0xFFF;
	tmp = tmp ^ (reduction << 6);
	tmp = tmp ^ (reduction << 4);
	tmp = tmp ^ reduction << 1;
	tmp = tmp ^ reduction;
	//second step of reduction
	reduction = (tmp >> 12);
	tmp = tmp ^ (reduction << 6);
	tmp = tmp ^ (reduction << 4);
	tmp = tmp ^ reduction << 1;
	tmp = tmp ^ reduction;

	tmp = tmp & 0x1FFF;
	return tmp;
}

static inline gf relative_field_representation(gf a, int k) {
	gf x[extension] = { 0 };
	gf b_0_t = a & 0x1;
	gf b_1_t = (a & 0x2) >> 1;
	gf b_2_t = (a & 0x4) >> 2;
	gf b_3_t = (a & 0x8) >> 3;
	gf b_4_t = (a & 0x10) >> 4;
	gf b_5_t = (a & 0x20) >> 5;
	gf b_6_t = (a & 0x40) >> 6;
	gf b_7_t = (a & 0x80) >> 7;
	gf b_8_t = (a & 0x100) >> 8;
	gf b_9_t = (a & 0x200) >> 9;
	gf b_10_t = (a & 0x400) >> 10;
	gf b_11_t = (a & 0x800) >> 11;

	gf b_0 = b_0_t ^ b_2_t ^ b_6_t ^ b_7_t ^ b_8_t ^ b_9_t ^ b_10_t ^ b_11_t;

	gf b_1 = b_4_t ^ b_6_t ^ b_9_t ^ b_10_t;

	gf b_2 = b_2_t ^ b_3_t ^ b_5_t ^ b_6_t ^ b_8_t;

	gf b_3 = b_2_t ^ b_7_t ^ b_8_t ^ b_9_t;

	gf b_4 = b_2_t ^ b_5_t ^ b_7_t ^ b_8_t ^ b_9_t;

	gf b_5 = b_2_t ^ b_4_t ^ b_7_t ^ b_8_t ^ b_11_t;

	gf b_6 = b_1_t ^ b_2_t ^ b_4_t ^ b_5_t ^ b_7_t ^ b_11_t;

	gf b_7 = b_2_t ^ b_4_t ^ b_5_t ^ b_6_t ^ b_8_t ^ b_11_t;

	gf b_8 = b_4_t;

	gf b_9 = b_3_t ^ b_4_t ^ b_8_t ^ b_10_t ^ b_11_t;

	gf b_10 = b_3_t ^ b_5_t ^ b_8_t ^ b_9_t ^ b_11_t;

	gf b_11 = b_3_t ^ b_5_t ^ b_8_t ^ b_9_t;

	x[0] = (b_0) | (b_1 << 1) | (b_2 << 2) | (b_3 << 3) | (b_4 << 4)
			| (b_5 << 5);
	x[1] = (b_6) | (b_7 << 1) | (b_8 << 2) | (b_9 << 3) | (b_10 << 4)
			| (b_11 << 5);
	return x[k];
	return x[k];
}

static inline gf absolut_field_representation(gf *element) {
	gf beta = 197;
	gf tmp1 = 0, tmp2 = 0, in0 = element[0], in1 = element[1];

	for (int i = 0; i < 8; i++) {
		gf power = gf_pow_f_q_m(beta, i);
		if (in0 & (1 << i))
			tmp1 ^= power;
	}
	for (int i = 0; i < 8; i++) {
		gf power = gf_pow_f_q_m(beta, i);
		if (in1 & (1 << i))
			tmp2 ^= power;
	}

	gf tmp_3 = gf_q_m_mult(1, tmp1);
	gf tmp_4 = gf_q_m_mult(2, tmp2);

	gf result = tmp_3 ^ tmp_4;
	return result;

}
#endif

static inline gf gf_add(gf a, gf b) {
	return (a ^ b);
}

static inline gf gf_sum(gf a, gf b) {
	return (a ^ b);
}

#endif /* GF_H_ */
