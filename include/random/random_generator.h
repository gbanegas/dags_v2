/*
 * random_generator.h
 *
 *  Created on: May 3, 2018
 *      Author: vader
 */

#ifndef INCLUDE_RANDOM_RANDOM_GENERATOR_H_
#define INCLUDE_RANDOM_RANDOM_GENERATOR_H_
#include <stdlib.h>

#include "../definitions.h"
#include "../parameters/param.h"
#include "../random/rng.h"

extern void random_elements_fix(gf* set_of_elements_in_F_q_m);
extern void random_elements(gf* set_of_elements_in_F_q_m);

extern void get_element_F_q_m(gf *set_of_elements);

extern int get_random_int(int size);

extern void random_list_of_integer(int* buff, unsigned long xlen);

#endif /* INCLUDE_RANDOM_RANDOM_GENERATOR_H_ */
