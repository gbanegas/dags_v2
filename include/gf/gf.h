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

#include "../definitions.h"
#include "../parameters/param.h"

gf relative_field_representation(gf a, int k);
gf absolut_field_representation(gf* element);

gf gf_q_m_inv(gf in);
gf gf_pow_f_q_m(gf in, gf power);
gf gf_pow_f_q(gf in, gf power);

gf gf_inv(gf in);
gf gf_div(gf a, gf b);
gf gf_div_f_q_m(gf a, gf b);
void print_F_q_element(gf a);
void print_F_q_m_element(gf a);

#if defined(DAGS_3) || defined(DAGS_5) || defined(DAGS_TOY)
static inline uint8_t gf_mult(const gf in0, const  gf in1) {
	gf reduction;
	gf tmp;
	gf t0 = in0, t1 = in1;
	int i;

	//Multiplication
	tmp = 0;

	for (i = 0; i < 10; i++){
		tmp ^= (t0 * (t1 & (1 << i)));
	}

	//first step of reduction
	//second step of reduction
	for (i=0; i < 2; i++){
		reduction = (tmp >> 8) & 0x7F;
		tmp = tmp & 0xFF;
		tmp ^= reduction;
		tmp ^= reduction << 2;
		tmp ^= reduction << 3;
		tmp ^= reduction << 4;
	}
	
	return tmp;
}

static inline gf gf_q_m_mult(const gf in0, const  gf in1) {
	gf reduction; 
	uint32_t tmp;
	gf t0 = in0, t1 = in1;
	int i; 

	//Multiplication
	tmp = 0;

	for (i = 0; i < 18; i++){
		tmp ^= (t0 * (t1 & (1 << i)));
	}

	//first step of reduction
	\

	for (i = 0; i < 2; i++){
		reduction = (tmp >> 16) & 0x7FFF;
		tmp = tmp & 0xFFFF;
		tmp ^= reduction;
		tmp ^= reduction << 2;
		tmp ^= reduction << 3;
		tmp ^= reduction << 5;
	}

	return tmp;
}
#endif


#ifdef DAGS_1
static inline gf gf_mult(gf in0, gf in1) {
	uint64_t i, tmp, t0 = in0, t1 = in1;

	//Multiplication
	tmp = 0;

	for (i = 0; i < 7; i++)
		tmp ^= (t0 * (t1 & (1 << i)));

	//reduction
	tmp = tmp & 0xFFF;	// tmp & 0000 1111 1111 1111
	tmp ^= (tmp >> 6);
	tmp ^= (tmp >> 5) ;
	tmp = tmp & 0x3F;
	return tmp;
}

static inline gf gf_q_m_mult(gf in0, gf in1) {
	int i, tmp; 
	gf t0 = in0, t1 = in1;
	gf reduction
	//Multiplication
	tmp = t0 * (t1 & 1);

	for (i = 1; i < 18; i++)
		tmp ^= (t0 * (t1 & (1 << i)));

	//reduction
	tmp = tmp & 0x7FFFFF;	// tmp & 0111 1111 1111 1111
	//first step of reduction
	//second step of reduction
	
	for (i=0; i < 2; i++){
		reduction = (tmp >> 12);
		tmp = tmp & 0xFFF;
		tmp ^= reduction;
		tmp ^= reduction << 1;
		tmp ^= reduction << 4;
		tmp ^= reduction << 6;
	}
	
	tmp = tmp & 0xFFF;
	return tmp;
}
#endif


static inline gf gf_add(gf a, gf b) {
	return (a ^ b);
}

static inline gf gf_sum(gf a, gf b) {
	return (a ^ b);
}

#endif /* GF_H_ */
