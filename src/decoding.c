/*
 * decoding.c
 *
 *  Created on: Jun 2, 2018
 *      Author: vader
 */

#include "decoding.h"

int decoding(const gf* v, const gf* y, const unsigned char *c,
		unsigned char *error, unsigned char *code_word) {
	int i, k, j, dr;
	int st = (signature_block_size * pol_deg);
	polynomial *syndrom;
	polynomial *omega, *sigma, *re, *uu, *u, *quotient, *rest, *app, *temp,
			*temp_sum;
	polynomial *delta, *pos;
	gf pol_gf, tmp, tmp1, o;
	gf alpha;

	//Compute Syndrome normally
#ifdef DEBUG_P
	printf("Decoding:Computing syndrom");

	printf("\n");
#endif
	syndrom = create_polynomial(st - 1);
	compute_syndrom(v, y, c, syndrom);

	if (syndrom->degree == -1) {
		return -1;
	}

	//Resolution of the key equation
	re = create_polynomial(st);
	re->coefficient[st] = 1;

	polynomial_get_update_degree(re);

	app = create_polynomial(st);
	uu = create_polynomial(st);
	u = create_polynomial(st);
	u->coefficient[0] = 1;
	u->degree = 0;

	dr = syndrom->degree;
#ifdef DEBUG_P
	printf("Computing re,u,quotient");

	printf("\n");
#endif
	while (dr >= (st / 2)) {

		quotient = poly_quo(re, syndrom);
		rest = poly_rem(re, syndrom);

		poly_set(re, syndrom); // re = p

		poly_set(syndrom, rest); // p = mpd_re

		poly_set(app, uu); // app = UU

		poly_set(uu, u); //UU = U

		temp = poly_multiplication(u, quotient);
		temp_sum = polynomial_addition(temp, app);

		poly_set(u, temp_sum); // U = (U*quotient) + app
		polynomial_free(temp);
		polynomial_free(temp_sum);
		polynomial_free(quotient);
		polynomial_free(rest);

		dr = syndrom->degree;
	}

	polynomial_free(re);
	polynomial_free(uu);
	polynomial_free(app);

#ifdef DEBUG_P
	printf("Computing delta, omega, sigma");

	printf("\n");
#endif
	delta = create_polynomial(0);
	gf u_over_z = polynomial_evaluation(u, 0); // z=0
	delta->coefficient[0] = gf_q_m_inv(u_over_z);
	omega = poly_multiplication(syndrom, delta);
	polynomial_free(syndrom);
	sigma = poly_multiplication(u, delta);

	polynomial_free(delta);
	polynomial_free(u);

	gf aux_perm[F_q_m_size] = { 0 };
	for (i = 0; i < F_q_m_size; i++) {
		aux_perm[i] = gf_pow_f_q_m(2, i);
	}

	int aux_position[weight] = { -1 };
	j = 0;

#ifdef DEBUG_P
	printf("Computing error position");
	printf("\n");
#endif
	for (i = 0; i < F_q_m_size; i++) {
		gf result = polynomial_evaluation(sigma, aux_perm[i]);
		if (!result) {
			int pos =  index_of_element(v, gf_q_m_inv(aux_perm[i]));
#ifdef DEBUG_P
	printf("%d,", pos);
#endif
			aux_position[j] = pos;
			j += 1;
		}
	}
#ifdef DEBUG_P
	printf("\n");
#endif
	polynomial_free(sigma);

	//Element for determining the value of errors
	if (check_positions(aux_position, st / 2)) {
		return -1;
	}
#ifdef DEBUG_P
	printf("decoding error_position: ");
	for (int i = 0; i < weight; i++) {
		printf("%d, ", aux_position[i]);
	}
	printf("\n");
#endif
	pos = create_polynomial(st / 2);
	for (i = 0; i < st / 2; i++) {
		pos->coefficient[i] = (gf) aux_position[i];
	}
	polynomial_get_update_degree(pos);
	if (pos->degree == -1) {
		return -1;
	}

#ifdef DEBUG_P
	printf("Computing evaluating error position");
	printf("\n");
#endif
	/// H_alt = produit_matrix(H_alt,PP);
	app = create_polynomial(pos->degree);
	for (j = 0; j <= pos->degree; j++) {
		pol_gf = 1;
		for (i = 0; i <= pos->degree; i++) {
			if (i != j) {
				tmp = gf_q_m_mult(v[pos->coefficient[i]],
						gf_q_m_inv(v[pos->coefficient[j]]));
				tmp = gf_add(1, tmp);
				pol_gf = gf_q_m_mult(pol_gf, tmp);
			}
		}
		o = polynomial_evaluation(omega, gf_q_m_inv(v[pos->coefficient[j]]));
		tmp1 = gf_q_m_mult(y[pos->coefficient[j]], pol_gf);
		if (tmp1 != 0)
			app->coefficient[j] = gf_q_m_mult(o, gf_q_m_inv(tmp1));
	}
	polynomial_get_update_degree(app);
	polynomial_free(omega);
#if defined(DAGS_TOY) | defined(DAGS_3) | defined(DAGS_5)
	gf exp_alpha = (F_q_m_size - 1) / (F_q_size - 1);
	alpha = gf_pow_f_q_m(2, exp_alpha); //b^((q^m)-1)/(q-1)
#endif
#if defined(DAGS_1)
	alpha = 197;
#endif
#ifdef DEBUG_P
	printf("Reconstruction of the error vector");
	printf("\n");
#endif
	k = 0;
	//Reconstruction of the error vector
	for (i = 0; i <= app->degree; i++) {
		k = discrete_logarithm(app->coefficient[i], alpha);

		gf correction = gf_pow_f_q(2, k);
		error[pos->coefficient[i]] = correction;

	}
	polynomial_free(app);
	polynomial_free(pos);

#ifdef DEBUG_P
	printf("decoding_error:\n");
	/*for (i = 0; i < code_length; i++) {
		printf(" %" PRIu16 ", ", error[i]);
	}
	printf("\n");*/
#endif
	//Reconstruction of code_word
	for (i = 0; i < code_length; i++) {
		code_word[i] = (c[i] ^ error[i]) & F_q_m_order;
	}
	return 0;
}
