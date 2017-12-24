#include "gf.h"

/*
 ~~~~~~~~ARITHMETIC FIELD ELEMENT CONSTRUCTION ~~~~~~~~~~~~~~~~
 We define arithmetic field in  F[2][x]/(f), where f is an m-irreducible polynomial.
 In our case, m=5 or m=6.
 For multiplication ,inversion,square,exponentiation field elements, we have adopted the "bitsliced-operation" technic.
 Addition field element, we used XOR between integers.
 */


// Correct gf_mult
gf gf_mult(gf x, gf y) {
	//gf a1, b1, a2, b2, a3, b3;
	gf a3, b3;

	//a1 = (x >> gf_extd_sf);
	//b1 = (x & (u_val - 1));
	//a2 = (y >> gf_extd_sf);
	//b2 = (y & (u_val - 1));

	gf tmp = gf_mult_fast((x >> gf_extd_sf), (y >> gf_extd_sf));

	gf_p tmp1 = ((x >> gf_extd_sf) << 16) | (y >> gf_extd_sf);
	gf_p tmp2 = ((y & (u_val - 1)) << 16) | (x & (u_val - 1));
	//gf result_1 = gf_mult_fast_6(a1, b2);
	//gf result = gf_mult_fast_6(b1, a2);
	gf_p r = gf_mult_parallel_4(tmp1, tmp2);
	a3 = gf_mult_fast(tmp, 36) ^ r ^ (r >> 16);

	//a3 = gf_mult_fast_6(tmp, 36) ^ result ^ result_1;

	//printf("temp_result %"PRIu16"\n", temp_result);
	//printf("a3 %"PRIu16"\n", a3);

	gf_p tmp11 = (tmp << 16) | (x & (u_val - 1));
	gf_p tmp12 = (2 << 16) | (y & (u_val - 1));
	gf_p r1 = gf_mult_parallel_4(tmp11, tmp12);
	b3 = (r1 >> 16) ^ r1;

	//b3 = gf_mult_fast_6(tmp, 2) ^ gf_mult_fast_6(b1, b2);

	//printf("temp_result %"PRIu16"\n", temp_result);
	//printf("b3 %"PRIu16"\n", b3);
	return (a3 << gf_extd_sf) ^ b3;
}

// Correct gf_sq
gf gf_sq(gf x) {
	gf a1, b1, a3, b3;

	a1 = x >> gf_extd_sf;
	b1 = x & (u_val - 1);

	gf_p r = gf_mult_parallel_4((a1 << 16) | b1, ((a1) << 16) | b1);
	gf tt = r >> 16;

	gf_p r1 = gf_mult_parallel_4(((tt) << 16) | tt, (36 << 16) | 2);

	a3 = (r1 >> 16);
	b3 = (r1 & 0xFFF) ^ r;

	return (a3 << gf_extd_sf) ^ b3;
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

