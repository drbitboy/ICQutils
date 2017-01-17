#ifndef __SPUDPRINT_H__
#define __SPUDPRINT_H__
#include <stdio.h>
#include <stdlib.h> /* for getenv */
#include <string.h>
#include <math.h>
#ifdef vms
char *malloc();
#else
#include "localmalloc.h"
#endif

#include "spudshap.h"
#include "debug.h"

void spudprint_xplate( SPUDF *spudf);
void spudprint_stl( SPUDF *spudf, double shellthick);
void spudprint_oogl( SPUDF *spudf);
void spudprint_plateBareToFile( SPUDF *spudf, FILE *f);
void spudprint_plateBare( SPUDF *spudf);
void spudprint_plate( SPUDF *spudf);
void spudprint_raysh( SPUDR *spudr, SPUDF *spudf);
#endif /* __SPUDPRINT_H__ */
