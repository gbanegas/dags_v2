#include "../../include/structures/mxm.h"
#include "../cpucycles.h"

#define BLK_SIZE 128
#define min(a,b) (((a)<(b))?(a):(b))

#define bound 32


#if defined(DAGS_3) || defined(DAGS_5) || defined(DAGS_TOY)
void gf_q_m_mult_bound(const gf A, const  gf* restrict B, gf* restrict C) {
	gf reduction[bound]; 
	uint32_t tmp[bound] = {0};
	
	int i,j; 

	//Multiplication
	for (i = 0; i < 18; i++)	tmp[0] ^= (A * (B[0] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[1] ^= (A * (B[1] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[2] ^= (A * (B[2] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[3] ^= (A * (B[3] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[4] ^= (A * (B[4] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[5] ^= (A * (B[5] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[6] ^= (A * (B[6] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[7] ^= (A * (B[7] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[8] ^= (A * (B[8] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[9] ^= (A * (B[9] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[10] ^= (A * (B[10] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[11] ^= (A * (B[11] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[12] ^= (A * (B[12] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[13] ^= (A * (B[13] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[14] ^= (A * (B[14] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[15] ^= (A * (B[15] & (1 << i))); 
	// /*
	for (i = 0; i < 18; i++)	tmp[16+0]  ^= (A * (B[16+0] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+1]  ^= (A * (B[16+1] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+2]  ^= (A * (B[16+2] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+3]  ^= (A * (B[16+3] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+4]  ^= (A * (B[16+4] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+5]  ^= (A * (B[16+5] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+6]  ^= (A * (B[16+6] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+7]  ^= (A * (B[16+7] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+8]  ^= (A * (B[16+8] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+9]  ^= (A * (B[16+9] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+10] ^= (A * (B[16+10] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+11] ^= (A * (B[16+11] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+12] ^= (A * (B[16+12] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+13] ^= (A * (B[16+13] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+14] ^= (A * (B[16+14] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+15] ^= (A * (B[16+15] & (1 << i))); 

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

#endif

#ifdef DAGS_1
void gf_q_m_mult_bound(const gf A, const  gf* restrict B, gf* restrict C) {
	gf reduction[bound]; 
	uint32_t tmp[bound] = {0};
	
	int i,j; 
	
	//Multiplication
	for (i = 0; i < 18; i++)	tmp[0] ^= (A * (B[0] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[1] ^= (A * (B[1] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[2] ^= (A * (B[2] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[3] ^= (A * (B[3] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[4] ^= (A * (B[4] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[5] ^= (A * (B[5] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[6] ^= (A * (B[6] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[7] ^= (A * (B[7] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[8] ^= (A * (B[8] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[9] ^= (A * (B[9] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[10] ^= (A * (B[10] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[11] ^= (A * (B[11] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[12] ^= (A * (B[12] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[13] ^= (A * (B[13] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[14] ^= (A * (B[14] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[15] ^= (A * (B[15] & (1 << i))); 
	// /*
	for (i = 0; i < 18; i++)	tmp[16+0]  ^= (A * (B[16+0] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+1]  ^= (A * (B[16+1] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+2]  ^= (A * (B[16+2] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+3]  ^= (A * (B[16+3] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+4]  ^= (A * (B[16+4] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+5]  ^= (A * (B[16+5] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+6]  ^= (A * (B[16+6] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+7]  ^= (A * (B[16+7] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+8]  ^= (A * (B[16+8] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+9]  ^= (A * (B[16+9] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+10] ^= (A * (B[16+10] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+11] ^= (A * (B[16+11] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+12] ^= (A * (B[16+12] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+13] ^= (A * (B[16+13] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+14] ^= (A * (B[16+14] & (1 << i))); 
	for (i = 0; i < 18; i++)	tmp[16+15] ^= (A * (B[16+15] & (1 << i))); 

	//reduction
	//first step of reduction
	//second step of reduction

	for (j = 0; j < bound; j++){
		for (i = 0; i < 2; i++){
            reduction[j] = (tmp[j] >> 12) &0x7FF;
            tmp[j] = tmp[j] & 0xFFF;
            tmp[j] ^= reduction[j];
            tmp[j] ^= reduction[j] << 1;
            tmp[j] ^= reduction[j] << 4;
            tmp[j] ^= reduction[j] << 6;
        }
	}
	
	for (j = 0; j < bound; j++){
		C[j] ^=  (tmp[j] & 0xFFF);
	}
}

#endif


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

void mxm_block_reorder_reuse_unroll_bound(gf* restrict C, const gf* restrict A, const gf* restrict B, const int m, const int n, const int p) {
	int i, j, k, ii, jj, kk, Aik, bs = BLK_SIZE;

	for (ii = 0; ii < m; ii += bs)
		for (kk = 0; kk < n; kk += bs)
			for (jj = 0; jj < p; jj += bs)
				for (i = ii; i < min(m, ii + bs); i++)
					for (k = kk; k < min(n, kk + bs); k++) {
						Aik = A[n * i + k];
						for (j = jj; j < min(p, jj + bs); j += bound) {
							gf_q_m_mult_bound(Aik, &B[p * k + j], &C[p * i + j]);
							
						}
					}
}

// Cmxp = Amxn * Bnxp
void mxm_product(gf* restrict dest,const gf* restrict a, const gf* restrict b, const int m, const int n, const int p) {
	long long start, end; 
	start = cpucycles();
	// Option for GCC 
	mxm_block_reorder_reuse_unroll_bound(dest, a, b, m, n, p);
	// Option for CLANG
	// mxm_block_reorder_reuse(dest, a, b, m, n, p);
	end = cpucycles() - start;
	PRINT_DEBUG("[++++] timming %lld \n", end/1000000);
	
}
