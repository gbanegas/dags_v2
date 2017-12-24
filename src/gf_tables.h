/*
 * gf_tables.h
 *
 *  Created on: Dec 1, 2017
 *      Author: vader
 */

#ifndef SRC_GF_TABLES_H_
#define SRC_GF_TABLES_H_

#include <stdint.h>
#include "types_def.h"


gf_t gf_log_sf[64];
gf_t gf_antilog_sf[64];

gf gf_log[4096];
gf gf_antilog[4096];

#endif /* SRC_GF_TABLES_H_ */
