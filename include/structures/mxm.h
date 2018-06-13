#ifndef MXM_H

#define	MXM_H

#include <stdio.h>
#include <stdlib.h>

#include "../gf/gf.h"

void mxm_product(gf *dest, const gf *a, const gf *b, int m, int n, int p);

#endif  /* MXM_H */

