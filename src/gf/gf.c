#include "../include/gf/gf.h"

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
	//gf out = gf_pow_f_q(in, 254);
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
	gf out = gf_pow_f_q_m(in, 254);
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

