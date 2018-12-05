/*
 * standard.h
 *
 *  Created on: May 24, 2018
 *      Author: vader
 */

#ifndef SRC_MULTIPLICATIONS_STANDARD_H_
#define SRC_MULTIPLICATIONS_STANDARD_H_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "param.h"
#include "definitions.h"
#include "matrix_operations.h"
#include "matrix_utils.h"
#include "utils.h"
#include "gf.h"

extern void std_multiplication(int n, int *a, int *b, int *result);

extern void std_multiplication_gf(int n, gf *a, gf *b, gf *result);

#endif /* SRC_MULTIPLICATIONS_STANDARD_H_ */
