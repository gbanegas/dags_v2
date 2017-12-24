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

#include "gf_tables.h"
#include "types_def.h"
#include "param.h"

//int gf_extension_degree, gf_cardinality, gf_multiplicative_order;

//gf *gf_log;
//gf *gf_antilog;

#define gf_modq_1_sf(d) ((d) % (u_val-1))

#define _gf_modq_1(d) ((d) % (gf_ord))

static inline gf gf_mult_fast(gf in0, gf in1) {
	uint64_t i, tmp, t0 = in0, t1 = in1;

	//Multiplication
	tmp = t0 * (t1 & 1);

	for (i = 1; i < 7; i++)
		tmp ^= (t0 * (t1 & (1 << i)));

	//reduction
	tmp = tmp & 0xFFF; // tmp & 0000 0111 1111 1111
	tmp = tmp ^ (tmp >> 6);
	tmp = tmp ^ ((tmp >> 5) & 0x3E);
	tmp = tmp & 0x3F;
	return tmp;
}

static inline gf_p gf_mult_parallel_4(gf_p a, gf_p b) {
	uint64_t i, tmp, t0 = a, t1 = b;

	//Multiplication
	tmp = ( (t0 & 0xFF) * (t1 & 0x1) ) | ( (t0 & 0xFF0000) * (t1 & 0x10000));

	for (i = 1; i < 7; i++) {
		uint64_t ta = ((t0 & 0x7F) * (t1 & (1 << i)));
		uint64_t tb = ((t0 & 0x7F0000) * (t1 & (1 << (i + 16))));
		tmp ^=  ((ta ) | (tb));
	}

	//reduction
	tmp = tmp & 0xFFF00000FFF; // tmp & 0000 1111 1111 1111
	tmp = tmp ^ (tmp >> 6);
	tmp = tmp ^ ((tmp >> 5) & 0x3E0000003E);
	tmp = (tmp | (tmp >> 16));
	tmp = tmp & 0x3F003F;
	return tmp;
}


// gf_Pow_subfield is always calculate 2^k
#define gf_pow_subfield(x, i) (gf_antilog_sf[(gf_modq_1_sf(gf_log_sf[x] * i))])

// Inverse in the subfield
#define gf_inv_subfield(x) gf_antilog_sf[gf_ord_sf - gf_log_sf[x]]

gf gf_mult(gf in0, gf in1);

gf gf_inv(gf in);

gf gf_sq(gf in);


#endif /* GF_H_ */
