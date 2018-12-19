/*
 * main.c
 *
 *  Created on: May 3, 2018
 *      Author: Gustavo
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sodium.h>

#include "../include/api.h"
#include "../include/keygeneration.h"
#include "../include/encapsulation.h"
#include "../include/decapsulation.h"
#include "../include/gf/gf.h"
#include "../include/structures/polynomial.h"
#include "../include/definitions.h"

#include "cpucycles.h"

#define MAX_MARKER_LEN 50
#define KAT_SUCCESS 0
#define KAT_FILE_OPEN_ERROR -1
#define KAT_DATA_ERROR -3
#define KAT_CRYPTO_FAILURE -4

static const int EXECUTION_TIMES = 2;

int FindMarker(FILE *infile, const char *marker);
int ReadHex(FILE *infile, unsigned char *A, int Length, char *str);
void fprintBstr(FILE *fp, char *S, unsigned char *A, unsigned long long L);

long long todstart;
long long todend;
long long cpustart;
long long cpuend;
long long t_gen[1001];
long long t_enc[1001];
long long t_dec[1001];

long long t_gen_avg;
long long t_enc_avg;
long long t_dec_avg;

int main(void) {
	t_gen_avg = 0;
	t_enc_avg = 0;
	t_dec_avg = 0;
	if (sodium_init() < 0) {
		/* panic! the library couldn't be initialized, it is not safe to use */
		return 1;
	}
	const rlim_t kStackSize = 64L * 1024L * 1024L;   // min stack size = 64 Mb
	struct rlimit rl;
	int result;

	result = getrlimit(RLIMIT_STACK, &rl);
	if (result == 0) {
		if (rl.rlim_cur < kStackSize) {
			rl.rlim_cur = kStackSize;
			result = setrlimit(RLIMIT_STACK, &rl);
			if (result != 0) {
				fprintf(stderr, "setrlimit returned result = %d\n", result);
			}
		}
	}
	char fn_req[32], fn_rsp[32];
	FILE *fp_req, *fp_rsp;
	unsigned char seed[48];
	unsigned char random_values[48];
	unsigned char entropy_input[48];

	int count = 0;
	unsigned char pk[CRYPTO_PUBLICKEYBYTES] = { 0 };
	unsigned char ct[CRYPTO_CIPHERTEXTBYTES], ss[CRYPTO_BYTES]; //, ss1[CRYPTO_BYTES];
	unsigned char sk[CRYPTO_SECRETKEYBYTES] = { 0 };
	int byte_count = 48;
	int counter = 0;
	int i;
	// Create the REQUEST file
	sprintf(fn_req, "PQCkemKAT_%d.req", CRYPTO_SECRETKEYBYTES);
	if ((fp_req = fopen(fn_req, "w")) == NULL) {
		printf("Couldn't open <%s> for write\n", fn_req);
		return KAT_FILE_OPEN_ERROR;
	}
	sprintf(fn_rsp, "PQCkemKAT_%d.rsp", CRYPTO_SECRETKEYBYTES);
	if ((fp_rsp = fopen(fn_rsp, "w")) == NULL) {
		printf("Couldn't open <%s> for write\n", fn_rsp);
		return KAT_FILE_OPEN_ERROR;
	}

	// Read from /dev/urandom to initialize entropy for random_bytes()

	FILE *fp_urandom;
	fp_urandom = fopen("/dev/urandom", "r");
	if (0 == fread(&entropy_input, 1, byte_count, fp_urandom)) {
		PRINT_DEBUG("Failed to read in entropy input\n");
		return EXIT_FAILURE;
	}
	fclose(fp_urandom);

	randombytes_init(entropy_input, NULL, 256);
	for (i = 0; i < EXECUTION_TIMES; i++) {
		fprintf(fp_req, "count = %d\n", i);
		randombytes_NIST(random_values, 48);
		fprintBstr(fp_req, "seed = ", random_values, 48);
		fprintf(fp_req, "pk =\n");
		fprintf(fp_req, "sk =\n");
		fprintf(fp_req, "ct =\n");
		fprintf(fp_req, "ss =\n\n");
	}
	fclose(fp_req);

	//Create the RESPONSE file based on what's in the REQUEST file
	if ((fp_req = fopen(fn_req, "r")) == NULL) {
		printf("Couldn't open <%s> for read\n", fn_req);
		return KAT_FILE_OPEN_ERROR;
	}

	fprintf(fp_rsp, "# %s\n\n", CRYPTO_ALGNAME);

	//sk = malloc(CRYPTO_SECRETKEYBYTES);

	if (EOF == fscanf(fp_req, "%d", &count)) {
		PRINT_DEBUG("Failed to fscanf %d:%s\n", errno, strerror(errno));
		return EXIT_FAILURE;
	}

	fprintf(fp_rsp, "count = %d\n", count);

	if (!ReadHex(fp_req, seed, 48, "seed = ")) {
		return KAT_DATA_ERROR;
	}
	fprintBstr(fp_rsp, "seed = ", seed, 48);

	randombytes_init(seed, NULL, 256);
	int success = 0;
	int fails = 0;

	do {
		int ret_val = 0;
		printf("Starting iteration: %d\n", counter);

		long long start = cpucycles();
		//key_gen(v, y, &G);
		ret_val = key_pair_generation(pk, sk);
		//int ret_val = key_pair_generation(pk, sk);
		long long final = cpucycles();
		t_gen_avg = t_gen_avg + (final - start);
		printf("KeyGen: %lld\n", (final - start) / 1000000);

		start = cpucycles();
		ret_val = encapsulation(ct, ss, pk);
		//encrypt(ct, ss, &G);
		final = cpucycles();

		t_enc_avg = t_enc_avg + (final - start);

		printf("Encaps: %lld\n", (final - start) / 1000000);
		if (ret_val != 0) {
			printf("fail encrypt");
			return KAT_CRYPTO_FAILURE;
		}
		start = cpucycles();
		ret_val = decapsulation(ss, ct, sk);
		//ret_val = decrypt(ss, ct, v_k, y_k);
		final = cpucycles();
		t_dec_avg = t_dec_avg + (final - start);

		printf("decaps: %lld\n", (final - start) / 1000000);
		if (ret_val != 0) {
			printf("FAIL\n");
			fails++;
			//return -1;
		} else {
			printf("SUCCESS\n\n");
			success++;
		}
		counter++;

	} while (counter < 100);
	printf("SUCCESS: %d\n", success);
	printf("fails: %d\n", fails);
	printf("gen_avg: %lld\n", (t_gen_avg / counter));
	printf("enc_avg: %lld\n", (t_enc_avg / counter));
	printf("dec_avg: %lld\n", (t_dec_avg / counter));

//free(sk);
	return 0;
}

int FindMarker(FILE *infile, const char *marker) {
	char line[MAX_MARKER_LEN];
	int i, len;
	int curr_line;

	len = (int) strlen(marker);
	if (len > MAX_MARKER_LEN - 1)
		len = MAX_MARKER_LEN - 1;

	for (i = 0; i < len; i++) {
		curr_line = fgetc(infile);
		line[i] = curr_line;
		if (curr_line == EOF)
			return 0;
	}
	line[len] = '\0';

	while (1) {
		if (!strncmp(line, marker, len))
			return 1;

		for (i = 0; i < len - 1; i++)
			line[i] = line[i + 1];
		curr_line = fgetc(infile);
		line[len - 1] = curr_line;
		if (curr_line == EOF)
			return 0;
		line[len] = '\0';
	}

// shouldn't get here
	return 0;
}
int ReadHex(FILE *infile, unsigned char *A, int Length, char *str) {
	int i, ch, started;
	unsigned char ich;

	if (Length == 0) {
		A[0] = 0x00;
		return 1;
	}
	memset(A, 0x00, Length);
	started = 0;
	if (FindMarker(infile, str))
		while ((ch = fgetc(infile)) != EOF) {
			if (!isxdigit(ch)) {
				if (!started) {
					if (ch == '\n')
						break;
					else
						continue;
				} else
					break;
			}
			started = 1;
			if ((ch >= '0') && (ch <= '9'))
				ich = ch - '0';
			else if ((ch >= 'A') && (ch <= 'F'))
				ich = ch - 'A' + 10;
			else if ((ch >= 'a') && (ch <= 'f'))
				ich = ch - 'a' + 10;
			else
				// shouldn't ever get here
				ich = 0;

			for (i = 0; i < Length - 1; i++)
				A[i] = (A[i] << 4) | (A[i + 1] >> 4);
			A[Length - 1] = (A[Length - 1] << 4) | ich;
		}
	else
		return 0;

	return 1;
}

void fprintBstr(FILE *fp, char *S, unsigned char *A, unsigned long long L) {
	unsigned long long i;

	fprintf(fp, "%s", S);

	for (i = 0; i < L; i++)
		fprintf(fp, "%02X", A[i]);

	if (L == 0)
		fprintf(fp, "00");

	fprintf(fp, "\n");
}
