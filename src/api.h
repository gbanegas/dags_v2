/*********************************************************************************************
* DAGS: Key Encapsulation using Dyadic GS Codes.                              *
* This code is exclusively intended for submission to the NIST Post=Quantum Cryptography.    *
* For any other usage , contact the author(s) to ask permission.                             *
**********************************************************************************************
*/
#ifndef __API_H_INCLUDED__
#define __API_H_INCLUDED__

#define CRYPTO_SECRETKEYBYTES 6336 //3314  //4418 //   8642  //3313.5
#define CRYPTO_PUBLICKEYBYTES 11616   //991232
#define CRYPTO_CIPHERTEXTBYTES 2155 // bufferoverflow line 120 in encapsulation.c    // 1616
#define CRYPTO_BYTES 64

#define CRYPTO_ALGNAME "DAGS_5"

int crypto_kem_keypair(
    unsigned char *pk,
    unsigned char *sk);

int crypto_kem_enc(
    unsigned char *ct,
    unsigned char *ss,
    const unsigned char *pk);

int crypto_kem_dec(
    unsigned char *ss,
    const unsigned char *ct,
    const unsigned char *sk);

#endif
