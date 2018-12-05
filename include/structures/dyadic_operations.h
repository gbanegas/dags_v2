/*
 * dyadic_inverse.h
 *
 *  Created on: May 8, 2018
 *      Author: vader
 */

#ifndef SRC_DYADIC_OPERATIONS_H_
#define SRC_DYADIC_OPERATIONS_H_

#include <stdlib.h>
#include <math.h>

#include "../definitions.h"
#include "../gf/gf.h"
#include "../parameters/param.h"

#include "../karatsuba/karatsuba_dyadic.h"
#include "../karatsuba/standard.h"
#include "../karatsuba/sylvester_hadamard.h"

extern void dyadic_inverse(int n , gf *sig, gf *signature_inverse);

extern void dyadic_sum(int n , gf *signature_a, gf *signature_b, gf *signature_result);

extern void dyadic_product(int n , gf *signature_a, gf *signature_b, gf *signature_result);

#endif /* SRC_DYADIC_OPERATIONS_H_ */
