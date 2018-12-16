#include "../../include/gf/gf.h"

#if defined(DAGS_3) || defined(DAGS_5) || defined(DAGS_TOY)
gf relative_field_representation(gf a, int k) {
	gf x[extension] = {0};
	uint8_t b_0_t = a & 0x1;
	uint8_t b_1_t = (a & 0x2) >> 1;
	uint8_t b_2_t = (a & 0x4) >> 2;
	uint8_t b_3_t = (a & 0x8) >> 3;
	uint8_t b_4_t = (a & 0x10) >> 4;
	uint8_t b_5_t = (a & 0x20) >> 5;
	uint8_t b_6_t = (a & 0x40) >> 6;
	uint8_t b_7_t = (a & 0x80) >> 7;
	uint8_t b_8_t = (a & 0x100) >> 8;
	uint8_t b_9_t = (a & 0x200) >> 9;
	uint8_t b_10_t = (a & 0x400) >> 10;
	uint8_t b_11_t = (a & 0x800) >> 11;
	uint8_t b_12_t = (a & 0x1000) >> 12;
	uint8_t b_13_t = (a & 0x2000) >> 13;
	uint8_t b_14_t = (a & 0x4000) >> 14;
	uint8_t b_15_t = (a & 0x8000) >> 15;


	uint8_t A = b_5_t ^ b_9_t ^ b_11_t ^ b_12_t ^ b_13_t ^ b_15_t;
	uint8_t B = b_2_t ^ b_3_t ^ b_4_t ^ b_5_t;
	uint8_t C = b_6_t ^ b_8_t;
	uint8_t D = b_7_t ^ b_9_t;
	uint8_t E = b_10_t ^ b_11_t;

	uint8_t b_4  = C ^ D ^ b_10_t; // 4 
	uint8_t b_11 = b_5_t ^ b_6_t ^ E ^ b_15_t; // 4
	uint8_t b_9  = b_3_t ^ b_8_t ^ A; // 7
	uint8_t b_5  = b_4_t ^ b_4 ^ A; // 8
	uint8_t b_6  = b_4 ^ b_12_t ^ b_14_t; // 6
	uint8_t b_15 = b_4 ^ b_6_t ^ b_11_t ^ b_15_t; // 5 
	uint8_t b_3  = b_3_t ^ b_6_t ^ b_8_t ^ b_9_t ^ b_11 ; // 6
	uint8_t b_13 = b_4 ^ E ^ b_5_t ^ b_13_t; // 6
	uint8_t b_10 = b_2_t ^ b_4_t ^ b_11_t ^ b_14_t ^ b_15_t; // 4
	uint8_t b_12 = B ^ b_4 ^ b_4_t^ b_8_t ^ b_12_t ^ b_10; //  9
	uint8_t b_1  = B ^ b_8_t ^ b_11_t ^ b_13_t ^ b_14_t; // 7
	uint8_t b_0  = E ^ b_0_t ^ b_8_t ^ b_9_t ^ b_12_t; // 5
	uint8_t b_2  = C ^ b_4_t ^ b_11_t ^ b_13_t ^ b_14_t; // 5 
	uint8_t b_14 = C ^ b_4_t ^ b_7_t ^ b_12_t ^ b_13_t ^ b_14_t ^ b_15_t; // 7
	uint8_t b_7  = D ^ b_5_t ^  b_8_t ^ b_13_t ^ b_14_t ^ b_15_t; // 6
	uint8_t b_8  = B ^ b_5_t ^ b_1_t ^ b_7_t ^ b_10_t ^ b_12_t ^ b_13_t ^ b_15_t; // 8 

	x[0] = (b_0) | (b_1 << 1) | (b_2 << 2) | (b_3 << 3) | (b_4 << 4) | (b_5 << 5) | (b_6 << 6) | (b_7 << 7);
	x[1] = (b_8) | (b_9 << 1) | (b_10 << 2) | (b_11 << 3) | (b_12 << 4) | (b_13 << 5) | (b_14 << 6) | (b_15 << 7);
	return x[k];
}

gf absolut_field_representation(gf *element) {
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

//TODO: Change this
gf gf_pow_f_q_m(gf in, gf power) {
	gf h, t;
	h = 1;
	t = in;
	while (power != 0) {
		if ((power & 1) == 1) {
			h = gf_q_m_mult(h, t);
		}
		power = power >> 1;
		t = gf_q_m_mult(t, t);
	}
	return h;
}

gf gf_pow_f_q(gf in, gf power) {
	gf h, t;
	h = 1;
	t = in;
	while (power != 0) {
		if ((power & 1) == 1) {
			h = gf_mult(h, t);
		}
		power = power >> 1;
		t = gf_mult(t, t);
	}
	return h;
}
/*
 // Correct gf_sq
 gf gf_sq(gf x) {
 gf a1, b1, a3, b3, tmp1;

 a1 = x >> subfield;
 b1 = x & (F_q_size - 1);

 tmp1 = gf_mult(a1, a1);

 a3 = gf_mult(tmp1, 36);

 b3 = gf_mult(tmp1, 2) ^ gf_mult(b1, b1);

 return (a3 << subfield) ^ b3;
 }
 */

gf gf_div_f_q_m(gf a, gf b) {
	gf res = gf_q_m_mult(a, gf_q_m_inv(b));
	return res;
}

gf gf_div(gf a, gf b) {
	gf res = gf_mult(a, gf_inv(b));
	return res;
}

#if defined(DAGS_3) || defined(DAGS_5) || defined(DAGS_TOY)
// Correct gf_Inv
gf gf_inv(gf in) {
	gf out = gf_pow_f_q(in, 254);
	/*
	gf tmp_11;
	gf tmp_111;
	gf tmp_1111;

	gf out = in;
	out = gf_mult(out, out); //a^2
	tmp_111 = out;//a^2
	tmp_11 = gf_mult(out, in);//a^2*a = a^3

	out = gf_mult(tmp_11, tmp_11);//(a^3)^2 = a^6
	out = gf_mult(out, out);// (a^6)^2 = a^12
	tmp_111 = gf_mult(out, tmp_111);//a^12*a^2 = a^14
	tmp_1111 = gf_mult(out, tmp_11);//a^12*a^3 = a^15

	out = gf_mult(tmp_1111, tmp_1111);//(a^15)^2 = a^30
	out = gf_mult(out, out);//(a^30)^2 = a^60
	out = gf_mult(out, out);//(a^60)^2 = a^120
	out = gf_mult(out, out);//(a^120)^2 = a^240
	out = gf_mult(out, tmp_111);//a^240*a^14 = a^254
	*/
	return out;

}

gf gf_q_m_inv(gf in) {
	gf out = gf_pow_f_q_m(in, 65534);
	/*gf tmp_11;
	 gf tmp_111;
	 gf tmp_1111;

	 gf out = in;
	 gf tmp_a_2 = gf_q_m_mult(out, out); //a^2
	 tmp_111 = tmp_a_2; //a^2
	 tmp_11 = gf_q_m_mult(tmp_a_2, in); //a^2*a = a^3

	 out = gf_q_m_mult(tmp_11, tmp_11);//a^3*a^3 = a^6

	 out = gf_q_m_mult(out, out);//a^6*a^6 = a^12
	 gf tmp_a14 = gf_q_m_mult(out, tmp_a_2); //a^14
	 out = gf_q_m_mult(out, out);//a^3*a^6 = a^24
	 gf tmp_b = gf_q_m_mult(out, out);//a^3*a^6 = a^48
	 out = gf_q_m_mult(out, tmp_b);//a^3*a^6 = a^96

	 out = gf_q_m_mult(out, out);//a^3*a^6 = a^192

	 gf tmp_a = gf_q_m_mult(out, tmp_b); //a^240

	 out = gf_q_m_mult(out, out);//a^3*a^6 = a^384

	 gf tmp_768 = gf_q_m_mult(out, out);//a^3*a^6 = a^768
	 out = gf_q_m_mult(out, tmp_768);//a^3*a^6 = a^1536
	 tmp_11 = gf_q_m_mult(out, out);//a^3*a^6 = a^3072
	 out = gf_q_m_mult(out, tmp_11);//a^3*a^6 = a^6144
	 tmp_111 = gf_q_m_mult(out, out);//a^3*a^6 = a^12288
	 out = gf_q_m_mult(out, tmp_111);//a^3*a^6 = a^24576
	 out = gf_q_m_mult(out, out);//a^3*a^6 = a^49152

	 gf tmp_l = gf_q_m_mult(tmp_a, tmp_a14);//a^254
	 out = gf_q_m_mult(out, tmp_l); // 49406
	 tmp_1111 = gf_q_m_mult(tmp_111, tmp_11);//a^15360
	 out = gf_q_m_mult(out, tmp_1111);//64766
	 out = gf_q_m_mult(out, tmp_768);//65534*/

	return out;

}
#endif

#ifdef DAGS_1
gf gf_sq(gf x) {
	gf a1, b1, a3, b3, tmp1;

	a1 = x >> subfield;
	b1 = x & (F_q_size - 1);

	tmp1 = gf_mult(a1, a1);

	a3 = gf_mult(tmp1, 36);

	b3 = gf_mult(tmp1, 2) ^ gf_mult(b1, b1);

	return (a3 << subfield) ^ b3;
}

// Correct gf_Inv
gf gf_inv(gf in) {
	gf tmp_11;
	gf tmp_1111;

	gf out = in;
	out = gf_sq(out); //a^2
	tmp_11 = gf_mult(out, in); //a^2*a = a^3

	out = gf_sq(tmp_11); //(a^3)^2 = a^6
	out = gf_sq(out); // (a^6)^2 = a^12
	tmp_1111 = gf_mult(out, tmp_11); //a^12*a^3 = a^15

	out = gf_sq(tmp_1111); //(a^15)^2 = a^30
	out = gf_sq(out); //(a^30)^2 = a^60
	out = gf_sq(out); //(a^60)^2 = a^120
	out = gf_sq(out); //(a^120)^2 = a^240
	out = gf_mult(out, tmp_1111); //a^240*a^15 = a^255

	out = gf_sq(out); // (a^255)^2 = 510
	out = gf_sq(out); //(a^510)^2 =  1020
	out = gf_mult(out, tmp_11); //a^1020*a^3 = 1023

	out = gf_sq(out); //(a^1023)^2 = 2046
	out = gf_mult(out, in); //a^2046*a = 2047
	//gf t = gf_sq(out); //(a^2047)^2 = 4094
	/*
	 gf tmp = gf_pow(in, 4094);
	 //gf tmp = gf_pow(in, 4094);*/
	return gf_sq(out);
}

gf gf_q_m_inv(gf in) {
	gf out = gf_pow_f_q_m(in, 4094);
	return out;
}
#endif

void print_F_q_element(gf a) {
	for (int j = 7; j > -1; j--) {
		int l = a & (1 << j);
		if (l) {
			printf("a^%d + ", j);
		}
	}
}

void print_F_q_m_element(gf a) {
	for (int j = 15; j > -1; j--) {
		int l = a & (1 << j);
		if (l) {
			printf("b^%d + ", j);
		}
	}
}

