#ifndef MXM_H

#define	MXM_H

#include <stdio.h>
#include <stdlib.h>

#include "../gf/gf.h"

void mxm_product(gf* restrict dest,const gf* restrict a,const gf*  restrict b, const int m, const int n, const int p);

#endif  /* MXM_H */

