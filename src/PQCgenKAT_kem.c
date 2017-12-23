//
//  PQCgenKAT_kem.c
//
//  Created by Bassham, Lawrence E (Fed) on 8/29/17.
//  Copyright © 2017 Bassham, Lawrence E (Fed). All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>

#include "rng.h"
#include "api.h"
#include "cycles.h"

#define MAX_MARKER_LEN 50
#define KAT_SUCCESS 0
#define KAT_FILE_OPEN_ERROR -1
#define KAT_DATA_ERROR -3
#define KAT_CRYPTO_FAILURE -4

int FindMarker(FILE *infile, const char *marker);
int ReadHex(FILE *infile, unsigned char *A, int Length, char *str);
void fprintBstr(FILE *fp, char *S, unsigned char *A, unsigned long long L);

int main()
{
    char fn_req[32], fn_rsp[32];
    FILE *fp_req, *fp_rsp;
    unsigned char seed[48];
    unsigned char random_values[48];
    unsigned char entropy_input[48];
    unsigned char ct[CRYPTO_CIPHERTEXTBYTES], ss[CRYPTO_BYTES], ss1[CRYPTO_BYTES];
    int count = 0;
    unsigned char pk[CRYPTO_PUBLICKEYBYTES];
    unsigned char *sk;
    int ret_val;
    int byte_count = 48;
    int counter = 0;
    int i;
    uint64_t initial_key_pair, final_key_pair, key_pair_count;

    // Create the REQUEST file
    sprintf(fn_req, "PQCkemKAT_%d.req", CRYPTO_SECRETKEYBYTES);
    if ((fp_req = fopen(fn_req, "w")) == NULL)
    {
        printf("Couldn't open <%s> for write\n", fn_req);
        return KAT_FILE_OPEN_ERROR;
    }
    sprintf(fn_rsp, "PQCkemKAT_%d.rsp", CRYPTO_SECRETKEYBYTES);
    if ((fp_rsp = fopen(fn_rsp, "w")) == NULL)
    {
        printf("Couldn't open <%s> for write\n", fn_rsp);
        return KAT_FILE_OPEN_ERROR;
    }

    // Read from /dev/urandom to initialize entropy for random_bytes()

    FILE *fp_urandom;
    fp_urandom = fopen("/dev/urandom", "r");
    fread(&entropy_input, 1, byte_count, fp_urandom);
    fclose(fp_urandom);

    randombytes_init(entropy_input, NULL, 256);
    for ( i = 0; i < 100; i++)
    {
        fprintf(fp_req, "count = %d\n", i);
        randombytes(random_values, 48);
        fprintBstr(fp_req, "seed = ", random_values, 48);
        fprintf(fp_req, "pk =\n");
        fprintf(fp_req, "sk =\n");
        fprintf(fp_req, "ct =\n");
        fprintf(fp_req, "ss =\n\n");
    }
    fclose(fp_req);

    //Create the RESPONSE file based on what's in the REQUEST file
    if ((fp_req = fopen(fn_req, "r")) == NULL)
    {
        printf("Couldn't open <%s> for read\n", fn_req);
        return KAT_FILE_OPEN_ERROR;
    }

    fprintf(fp_rsp, "# %s\n\n", CRYPTO_ALGNAME);

    do
    {
        sk = malloc(CRYPTO_SECRETKEYBYTES);
        printf("Starting iteration: %d\n", counter);

        fscanf(fp_req, "%d", &count);

        fprintf(fp_rsp, "count = %d\n", count);

        if (!ReadHex(fp_req, seed, 48, "seed = "))
        {
            return KAT_DATA_ERROR;
        }
        fprintBstr(fp_rsp, "seed = ", seed, 48);

        randombytes_init(seed, NULL, 256);

        // Generate the public/private keypair
        do
        {
            initial_key_pair = rdtsc();
            ret_val = crypto_kem_keypair(pk, sk);
            final_key_pair = rdtsc();
            key_pair_count = final_key_pair - initial_key_pair;
        } while (ret_val != 0);

        printf("Key Generation cycles: %" PRId64 "\n", key_pair_count);

        if (ret_val != 0)
        {
            return KAT_CRYPTO_FAILURE;
        }

        fprintBstr(fp_rsp, "pk = ", pk, CRYPTO_PUBLICKEYBYTES);
        fprintBstr(fp_rsp, "sk = ", sk, CRYPTO_SECRETKEYBYTES);

        // Encapsulation
        uint64_t initial_enc = rdtsc();
        ret_val = crypto_kem_enc(ct, ss, pk);
        uint64_t final_enc = rdtsc();

        uint64_t enc_count = final_enc - initial_enc;

        printf("Encapsulation cycles: %" PRId64 "\n", enc_count);

        if (ret_val != 0)
        {
            printf("Encap got wrong");
            return KAT_CRYPTO_FAILURE;
        }

        fprintBstr(fp_rsp, "ct = ", ct, CRYPTO_CIPHERTEXTBYTES);
        fprintBstr(fp_rsp, "ss = ", ss, CRYPTO_BYTES);
        fprintf(fp_rsp, "\n");

        // Decapsulation
        uint64_t initial_key_dec = rdtsc();
        ret_val = crypto_kem_dec(ss1, ct, sk);
        uint64_t final_dec = rdtsc();

        uint64_t dec_count = final_dec - initial_key_dec;

        printf("Decapsulation cycles: %" PRId64 "\n", dec_count);

        uint64_t total_cycles = dec_count + key_pair_count + enc_count;
        printf("Total cycles: %" PRId64 "\n\n", total_cycles);

        if ((ret_val) != 0)
        {
            printf("Decap got wrong");
            return KAT_CRYPTO_FAILURE;
        }

        if (memcmp(ss, ss1, CRYPTO_BYTES))
        {
            printf("memcpy got wrong");
            return KAT_CRYPTO_FAILURE;
        }
        counter++;
        free(sk);
    
    } while (counter < 100);

    fclose(fp_req);
    fclose(fp_rsp);

    return KAT_SUCCESS;
}

//
// ALLOW TO READ HEXADECIMAL ENTRY (KEYS, DATA, TEXT, etc.)
//
//
// ALLOW TO READ HEXADECIMAL ENTRY (KEYS, DATA, TEXT, etc.)
//
int FindMarker(FILE *infile, const char *marker)
{
    char line[MAX_MARKER_LEN];
    int i, len;
    int curr_line;

    len = (int)strlen(marker);
    if (len > MAX_MARKER_LEN - 1)
        len = MAX_MARKER_LEN - 1;

    for (i = 0; i < len; i++)
    {
        curr_line = fgetc(infile);
        line[i] = curr_line;
        if (curr_line == EOF)
            return 0;
    }
    line[len] = '\0';

    while (1)
    {
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

//
// ALLOW TO READ HEXADECIMAL ENTRY (KEYS, DATA, TEXT, etc.)
//
int ReadHex(FILE *infile, unsigned char *A, int Length, char *str)
{
    int i, ch, started;
    unsigned char ich;

    if (Length == 0)
    {
        A[0] = 0x00;
        return 1;
    }
    memset(A, 0x00, Length);
    started = 0;
    if (FindMarker(infile, str))
        while ((ch = fgetc(infile)) != EOF)
        {
            if (!isxdigit(ch))
            {
                if (!started)
                {
                    if (ch == '\n')
                        break;
                    else
                        continue;
                }
                else
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

void fprintBstr(FILE *fp, char *S, unsigned char *A, unsigned long long L)
{
    unsigned long long i;

    fprintf(fp, "%s", S);

    for (i = 0; i < L; i++)
        fprintf(fp, "%02X", A[i]);

    if (L == 0)
        fprintf(fp, "00");

    fprintf(fp, "\n");
}
