/*
 * random_generator.c
 *
 *  Created on: May 3, 2018
 *      Author: vader
 */

#include "../../include/random/random_generator.h"

void random_elements_fix(gf* set_of_elements_in_F_q_m) {
	gf random_set_of_elements_in_F_q_m[] = { 24531, 57323, 43273, 56025, 22976,
			57007, 25366, 27286, 49855, 43274, 2813, 25122, 9793, 44115, 9692,
			16000, 38086 };
	for (int i = 0; i < 17; i++) {
		set_of_elements_in_F_q_m[i] = random_set_of_elements_in_F_q_m[i];
	}
}

void get_element_F_q_m(gf *set_of_elements) {
	gf data[] = { 25299, 33947, 51495, 10439, 5737, 6254, 35366, 21887, 20962,
			14001, 15274, 50666, 38451, 63192, 31189, 61008, 61673 };
	for (int i = 0; i < 17; i++) {
		set_of_elements[i] = data[i];
	}
}

int get_random_int(int size) {
	unsigned char i[1] = { 0 };
	randombytes_NIST(i, 1);
	return (i[0] % size);

}

void random_elements(gf* set_of_elements_in_F_q_m) {
	int i, j, v;
	gf tmp;
	unsigned char *random_bytes = 0;
	random_bytes = malloc(F_q_m_size * sizeof(gf));
	randombytes_NIST(random_bytes, F_q_m_size * sizeof(gf));
	for (i = 1; i <= F_q_m_order; i++) {
		set_of_elements_in_F_q_m[i - 1] = i;
	}

	for (j = 0; j < F_q_m_order; j++) {
		v = (random_bytes)[j] % (j + 1);
		tmp = set_of_elements_in_F_q_m[j];
		set_of_elements_in_F_q_m[j] = set_of_elements_in_F_q_m[v];
		set_of_elements_in_F_q_m[v] = tmp;
	}

	//remove 0's
	for (i = 0; i < F_q_m_order - 1; i++) {
		if (set_of_elements_in_F_q_m[i] == 0) {
			set_of_elements_in_F_q_m[i] = set_of_elements_in_F_q_m[i + 1];
		}
	}
	free(random_bytes);
}

void random_list_of_integer(int* buff, unsigned long xlen) {
	int data[] = { 15975, 4235, 14026, 8704, 3292, 359, 12977, 15499, 980,
			16179, 14845, 1134, 13392, 11739, 14959 };
	for (int i = 0; i < 15; i++) {
		buff[i] = data[i];
	}
	/*int j;
	 unsigned char* random_bytes;
	 random_bytes = (unsigned char*) calloc(xlen, sizeof(unsigned char));
	 randombytes(random_bytes, xlen);

	 for (j = 0; j < xlen; j++) {
	 int temp = random_bytes[j] % (xlen);
	 buff[j] = temp;

	 }
	 free(random_bytes);*/

}
