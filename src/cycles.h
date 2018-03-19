/*
 * cycles.h
 *
 *  Created on: Nov 8, 2017
 *      Author: vader
 */

#ifndef SRC_CYCLES_H_
#define SRC_CYCLES_H_

#include <stdint.h>
uint64_t rdtsc()
{
    //unsigned int lo, hi;
    //__asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
    //return ((uint64_t)hi << 32) | lo;
    unsigned long long time;
    asm volatile(
                        "xor %%rax, %%rax\n\t"
                        "cpuid\n\t"
                        "rdtsc\n\t"
                        "shl $32, %%rdx\n\t"
                        "or %%rdx,%%rax\n\t"

                    :"=a"(time)::"rbx","rcx","rdx");
    return time;
}

#endif /* SRC_CYCLES_H_ */
