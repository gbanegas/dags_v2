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

#include "../parameters/param.h"
#include "../definitions.h"
#include "../structures/matrix_operations.h"
#include "../structures/matrix_utils.h"
#include "../gf/gf.h"

extern void std_multiplication(int n, int *a, int *b, int *result);

extern void std_multiplication_gf(int n, gf *a, gf *b, gf *result);

#endif /* SRC_MULTIPLICATIONS_STANDARD_H_ */
