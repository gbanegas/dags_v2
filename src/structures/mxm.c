#include "../../include/structures/mxm.h"

#define BLK_SIZE 128
#define min(a,b) (((a)<(b))?(a):(b))
#define bound 32

#include "../cpucycles.h"

void gf_q_m_mult_bound(const gf A, const  gf* restrict B, gf* restrict C) {
	gf reduction[bound]; 
	uint32_t tmp[bound] = {0};
	
	int i,j; 

	//Multiplication
	for (i = 0; i < 18; i++){
		tmp[0] ^= (A * (B[0] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[1] ^= (A * (B[1] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[2] ^= (A * (B[2] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[3] ^= (A * (B[3] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[4] ^= (A * (B[4] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[5] ^= (A * (B[5] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[6] ^= (A * (B[6] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[7] ^= (A * (B[7] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[8] ^= (A * (B[8] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[9] ^= (A * (B[9] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[10] ^= (A * (B[10] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[11] ^= (A * (B[11] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[12] ^= (A * (B[12] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[13] ^= (A * (B[13] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[14] ^= (A * (B[14] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[15] ^= (A * (B[15] & (1 << i)));
	}
	// /*
	
	for (i = 0; i < 18; i++){
		tmp[16+ 0] ^= (A * (B[0+16] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[16+ 1] ^= (A * (B[1+16] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[16+ 2] ^= (A * (B[2+16] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[16+ 3] ^= (A * (B[3+16] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[16+ 4] ^= (A * (B[4+16] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[16+ 5] ^= (A * (B[5+16] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[16+ 6] ^= (A * (B[6+16] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[16+ 7] ^= (A * (B[7+16] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[16+ 8] ^= (A * (B[8+16] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[16+ 9] ^= (A * (B[9+16] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[16+ 10] ^= (A * (B[10+16] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[16+ 11] ^= (A * (B[11+16] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[16+ 12] ^= (A * (B[12+16] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[16+ 13] ^= (A * (B[13+16] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[16+ 14] ^= (A * (B[14+16] & (1 << i)));
	}
	for (i = 0; i < 18; i++){
		tmp[16+ 15] ^= (A * (B[15+16] & (1 << i)));
	}
	// */
	for (j = 0; j < bound; j++){
		for (i = 0; i < 2; i++){
            reduction[j] = (tmp[j] >> 16) &0x7fff;
            tmp[j] = tmp[j] & 0xFFFF;
            tmp[j] ^= reduction[j];
            tmp[j] ^= reduction[j] << 2;
            tmp[j] ^= reduction[j] << 3;
            tmp[j] ^= reduction[j] << 5;
        }
	}

	for (j = 0; j < bound; j++){
		C[j] ^=  tmp[j];
	}
}


void mxm_block_reorder_reuse(gf* restrict C, const gf* restrict A, const gf* restrict B, const int m, const int n, const int p) {
	int i, j, k, ii, jj, kk, Aik, bs = BLK_SIZE;

	for (ii = 0; ii < m; ii += bs)
		for (kk = 0; kk < n; kk += bs)
			for (jj = 0; jj < p; jj += bs)
				for (i = ii; i < min(m, ii + bs); i++)
					for (k = kk; k < min(n, kk + bs); k++) {
						Aik = A[n * i + k];
						for (j = jj; j < min(p, jj + bs); j++)
							C[p * i + j] ^= gf_q_m_mult(Aik, B[p * k + j]);
					}
}

void mxm_block_reorder_reuse_unroll_8(gf* restrict C, const gf* restrict A, const gf* restrict B, const int m, const int n, const int p) {
	int i, j, k, ii, jj, kk, Aik, bs = BLK_SIZE;

	for (ii = 0; ii < m; ii += bs)
		for (kk = 0; kk < n; kk += bs)
			for (jj = 0; jj < p; jj += bs)
				for (i = ii; i < min(m, ii + bs); i++)
					for (k = kk; k < min(n, kk + bs); k++) {
						Aik = A[n * i + k];
						for (j = jj; j < min(p, jj + bs); j += 8) {
							gf_q_m_mult_bound(Aik, &B[p * k + j], &C[p * i + j]);
						}
					}
}

void mxm_block_reorder_reuse_unroll_16(gf* restrict C, const gf* restrict A, const gf* restrict B, const int m, const int n, const int p) {
	int i, j, k, ii, jj, kk, Aik, bs = BLK_SIZE;

	for (ii = 0; ii < m; ii += bs)
		for (kk = 0; kk < n; kk += bs)
			for (jj = 0; jj < p; jj += bs)
				for (i = ii; i < min(m, ii + bs); i++)
					for (k = kk; k < min(n, kk + bs); k++) {
						Aik = A[n * i + k];
						for (j = jj; j < min(p, jj + bs); j += 16) {
							gf_q_m_mult_bound(Aik, &B[p * k + j], &C[p * i + j]);
							
						}
					}
}

void mxm_block_reorder_reuse_unroll_32(gf* restrict C, const gf* restrict A, const gf* restrict B, const int m, const int n, const int p) {
	int i, j, k, ii, jj, kk, Aik, bs = BLK_SIZE;

	for (ii = 0; ii < m; ii += bs)
		for (kk = 0; kk < n; kk += bs)
			for (jj = 0; jj < p; jj += bs)
				for (i = ii; i < min(m, ii + bs); i++)
					for (k = kk; k < min(n, kk + bs); k++) {
						Aik = A[n * i + k];
						for (j = jj; j < min(p, jj + bs); j += 32) {
							gf_q_m_mult_bound(Aik, &B[p * k + j], &C[p * i + j]);
							
						}
					}
}

/*
void mxm(gf* restrict C, const gf* restrict A, const gf* restrict B, const int m, const int n, const int p, const int uf) {
	//mxm_naive(C, A, B, m, n, p);
	//mxm_block(C, A, B, m, n, p);
	//mxm_block_reorder(C, A, B, m, n, p);
	PRINT_DEBUG("m n p = %d %d %d\n", m, n, p);
	switch (uf) {
	case 16:
		mxm_block_reorder_reuse_unroll_16(C, A, B, m, n, p);
		break;
	case 8:
		mxm_block_reorder_reuse_unroll_8(C, A, B, m, n, p);
		break;
	case 4:
		mxm_block_reorder_reuse_unroll_4(C, A, B, m, n, p);
		break;
	case 2:
		mxm_block_reorder_reuse_unroll_2(C, A, B, m, n, p);
		break;
	default:
		mxm_block_reorder_reuse(C, A, B, m, n, p);
		break;
	}
}
*/

// Cmxp = Amxn * Bnxp
void mxm_product(gf* restrict dest,const gf* restrict a, const gf* restrict b, const int m, const int n, const int p) {
	long long start, end; 
	start = cpucycles();
	// mxm_block_reorder_reuse_unroll_8(dest, a, b, m, n, p);
	// mxm_block_reorder_reuse_unroll_16(dest, a, b, m, n, p);
	mxm_block_reorder_reuse_unroll_32(dest, a, b, m, n, p);
	// mxm_block_reorder_reuse(dest, a, b, m, n, p);
	// mxm_block_reorder_reuse_unroll_2(dest, a, b, m, n, p);
	end = cpucycles() - start;
	PRINT_DEBUG("[++++] timming %lld \n", end/1000000);
	/*
	int uf;

	if (p % 16 == 0)
		uf = 16;
	else if (p % 8 == 0)
		uf = 8;
	else if (p % 4 == 0)
		uf = 4;
	else if (p % 2 == 0)
		uf = 2;
	else
		uf = 0;

	printf("Matrix dimensions %dx%d\n", m, n);
	printf("Block size: %d\n", BLK_SIZE);
	printf("Unrolls: %d\n", uf);

	mxm(dest, a, b, m, n, p, uf);
	*/
}
