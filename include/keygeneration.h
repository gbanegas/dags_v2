/*
 * keygeneration.h
 *
 *  Created on: May 3, 2018
 *      Author: Gustavo Banegas
 */

#ifndef SRC_KEYGENERATION_H_
#define SRC_KEYGENERATION_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sodium.h>


#include "../include/parameters/param.h"
#include "../include/definitions.h"
#include "structures/matrix_operations.h"
#include "gf/gf.h"
#include "util/util.h"

/**
 * @brief Key pair generation
 * @param pk string that will be stored the public key
 * @param sk string that will be stored the secret key
 *
 * @return It will back 0 if it was possible to generate both keys
 * and it will return != 0 if it wasn't possible to generate keys.
 *
 */
extern int key_pair_generation(unsigned char *pk, unsigned char *sk);

extern void  key_gen(gf *v, gf *y, matrix *G);

extern int build_dyadic_signature(gf *dyadic_signature);

extern void build_support(gf *signature_h, gf *u, gf *v);

extern void build_cauchy_matrix(gf *u, gf *v, matrix *H_cauchy);

extern void build_trapdoor(const matrix *H_cauchy, const gf *v, const gf *u,
		gf *y, matrix *H);

extern void project_H_on_F_q(const matrix *H, matrix *Hbase);

extern int generate_public_key(const matrix *Hbase, matrix *G);

#endif /* SRC_KEYGENERATION_H_ */
