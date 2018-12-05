#include "../../karatsuba/sylvester_hadamard.h"

matrix *make_walsh_hadamard_matrix(int n) {
	matrix *h = make_matrix(n, n);
	int ii, xx, yy;

	h->data[0][0] = 1;

	for (ii = 2; ii <= n; ii *= 2) {
		//Top right quadrant.
		for (xx = 0; xx < (ii / 2); ++xx) {
			for (yy = (ii / 2); yy < ii; ++yy) {
				h->data[xx][yy] = h->data[xx][yy - (ii / 2)];
			}
		}
		//Bottom left quadrant.
		for (yy = 0; yy < (ii / 2); ++yy) {
			for (xx = (ii / 2); xx < ii; ++xx) {
				h->data[xx][yy] = h->data[xx - (ii / 2)][yy];
			}
		}
		//Bottom right quadrant, inverse of other quadrants.
		for (xx = (ii / 2); xx < ii; ++xx) {
			for (yy = (ii / 2); yy < ii; ++yy) {
				h->data[xx][yy] = h->data[xx - (ii / 2)][yy - (ii / 2)];
				if (h->data[xx][yy] == 1) {
					h->data[xx][yy] = -1;
				} else {
					h->data[xx][yy] = 1;
				}
			}
		}
	}
	return h;
}

void sylvester_hadamard_multiplication(int n, const int *a, int * result) {
	int k = 1 << n;
	int adata[k];
	int *u = adata;
	memcpy(u, a, sizeof(int) * k);
	int v = 1;
	for (int j = 0; j < n; j++) {
		int w = v;
		v = 2 * v;
		for (int i = 0; i < k - 1; i = i + v) {
			for (int l = 0; l < w; l++) {
				int s = u[i + l];
				int q = u[i + l + w];
				u[i + l] = s + q;
				u[i + l + w] = s - q;

			}
		}

	}
	memcpy(result, u, sizeof(int) * k);
}

void multiply_dyadic_convolution(int n, const int* a, const int* b, int* result) {

	int k = 1 << n;
	int v_t[k];
	int u_t[k];
	int w_t[k];

	sylvester_hadamard_multiplication(n, a, v_t);
	/*printf("v_t: %s", "\n");
	 for (int x = 0; x < k; x++) {
	 printf("%d\t", v_t[x]);
	 }
	 printf("%s", "\n");*/

	sylvester_hadamard_multiplication(n, b, u_t);

	/*printf("u_t: %s", "\n");
	 for (int x = 0; x < k; x++) {
	 printf("%d\t", u_t[x]);
	 }
	 printf("%s", "\n");*/

	for (int i = 0; i < k - 1; i++) {
		w_t[i] = v_t[i] * u_t[i];
	}
	int w[k];
	sylvester_hadamard_multiplication(n, w_t, w);

	for (int i = 0; i < k; i++) {
		//w[i] =  nearest* w[i];
		w[i] = w[i] >> n;
	}

	memcpy(result, w, sizeof(int) * k);

}

//It computes the Sylvester-Hadamard transform, with a being a vector of length k, whose elements are
//polynomials of length poly_length with coefficients over Z
void sylvester_hadamard_multiplication_polynomial(const int poly_length,
		const int n, const int *a, int * result) {

	int k = 1 << n;

	int v = 1;
	for (int j = 0; j < n; j++) {
		int w = v;
		v = 2 * v;
		for (int i = 0; i < k - 1; i = i + v) {
			for (int l = 0; l < w; l++) {
				for (int b = 0; b < poly_length; b++) {
					int s = a[(i + l) * poly_length + b];
					int q = a[(i + l + w) * poly_length + b];
					result[(i + l) * poly_length + b] = s + q;
					result[(i + l + w) * poly_length + b] = s - q;
				}
			}
		}

	}
}

//Dyadic convolution over GF
void multiply_dyadic_convolution_gf(int n, const gf* a, const gf* b, gf* result) {

	int k = 1 << n;

	//Expand a and b into polynomials with binary coefficients: every element of a and b is
	//turned into a polynomials with field entries (0 or 1)
	int poly_size = k * field;
	int poly_a[poly_size], poly_b[poly_size];
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < field; j++) {
			int mask = 1 << j;
			if (a[i] & mask) {
				poly_a[i * field + j] = 1;
			} else {
				poly_a[i * field + j] = 0;
			}
			if (b[i] & mask) {
				poly_b[i * field + j] = 1;
			} else {
				poly_b[i * field + j] = 0;
			}
		}
	}

	//Compute diagonal form of poly_a
	int transformed_poly_a[poly_size];
	int transformed_poly_b[poly_size];

	for (int i = poly_size; i--;) {
		transformed_poly_a[i] = 0;
		transformed_poly_b[i] = 0;
	}

	sylvester_hadamard_multiplication_polynomial(field, n, poly_a,
			transformed_poly_a);

	//Compute diagonal form of poly_b

	sylvester_hadamard_multiplication_polynomial(field, n, poly_b,
			transformed_poly_b);

	//Element-wise product of diagonal forms
	int c_poly_size = 2 * poly_size;
	int transformed_poly_c[c_poly_size];
	for (int j = 0; j < c_poly_size; j++)
		transformed_poly_c[j] = 0;
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < field; j++) {
			for (int l = 0; l < field; l++)
				transformed_poly_c[i * F_q_size + j + l] += transformed_poly_a[i
						* field + j] * transformed_poly_b[i * field + l];
		}
	}

	//Reverse diagonal form of transformed_poly_c
	int poly_c[c_poly_size];

	for (int i = c_poly_size; i--;) {
		poly_c[i] = 0;
	}

	sylvester_hadamard_multiplication_polynomial(F_q_size, n,
			transformed_poly_c, poly_c);

	//Division by k and mod 2 reduction
	for (int i = 0; i < c_poly_size; i++)
		poly_c[i] = (int) (poly_c[i] / k) % 2;

	//Return to GF(2^m) and mod reduction
	gf c[k];
	for (int i = 0; i < k; i++) {
		if (poly_c[i * F_q_size]) {
			c[i] = 1;
		} else {
			c[i] = 0;
		}
		for (int j = 0; j < F_q_size; j++) {
			if (poly_c[i * F_q_size + j]) {
				c[i] += (1 << j);
			}
		}

		//Reduction of c[i] into a GF(2^field) element
		gf tmp = c[i];
		tmp = tmp & 0xFFF; // tmp & 0000 0111 1111 1111
		tmp = tmp ^ (tmp >> 6);
		tmp = tmp ^ ((tmp >> 5) & 0x3E);
		c[i] = tmp & 0x3F;
	}

	memcpy(result, c, sizeof(gf) * k);

}
