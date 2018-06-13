/*
 * decoding.h
 *
 *  Created on: Jun 2, 2018
 *      Author: vader
 */

#ifndef INCLUDE_DECODING_H_
#define INCLUDE_DECODING_H_

#include <stdio.h>

#include "random/random_generator.h"

#include "gf/gf.h"
#include "structures/polynomial.h"
#include "structures/matrix_operations.h"
#include "parameters/param.h"

#include "definitions.h"


extern int decoding(const gf* v, const gf* y, const unsigned char *c, unsigned char *error,
               unsigned char *code_word);

#endif /* INCLUDE_DECODING_H_ */
