/*
 * util_debug.h
 *
 *  Created on: Dec 20, 2018
 *      Author: vader
 */

#ifndef INCLUDE_UTIL_UTIL_DEBUG_H_
#define INCLUDE_UTIL_UTIL_DEBUG_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../gf/gf.h"
#include "../definitions.h"

void store_matrix_in_file(matrix *m);

void store_vector_in_file(gf *v, int size);

void store_char_vector_in_file(unsigned char *v, int size);


#endif /* INCLUDE_UTIL_UTIL_DEBUG_H_ */
