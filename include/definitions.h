/*
 * definitions.h
 *
 *  Created on: May 3, 2018
 *      Author: vader
 */

#ifndef SRC_DEFINITIONS_H_
#define SRC_DEFINITIONS_H_

#include <stdint.h>

#define ERROR_ZERO_IN_A_POSITION -1

typedef uint16_t gf;
typedef uint32_t gf_p;

typedef int bool;
#define true 1
#define false 0

struct signature_block {
	gf* signature;
};

struct matrix {
    int rows; // number of rows
    int cols; // number of columns
    gf* data;
};
typedef struct matrix matrix;


struct polynomial {
  int degree;
  int size;
  gf * coefficient;
};
typedef struct polynomial polynomial;

#endif /* SRC_DEFINITIONS_H_ */
