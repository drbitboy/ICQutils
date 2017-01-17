/* Create triaxial ellipsoid as Gaskell ICQ model
 *
 * Compile:
 *
 *   cc -o gaskelltriax -I.../cspice/include gaskelltriax.c .../cspice/lib/cspice.a -lm
 *
 * Usage:
 *
 *   ./gaskeltriax A B C Nicq > triax.icq
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SpiceUsr.h"
#include "gaskell.h"

const char Usage[] = { "Usage:  triax X Y Z q" };

void
xyf2u( SpiceInt q, SpiceDouble x, SpiceDouble y, SpiceInt face
     , SpiceDouble axSqrt[3], SpiceDouble v[3]) {
SpiceDouble dt[3];
SpiceInt k, j;

  vpack_c( tan((((2.0*x)/q)-1.0)*M_PI_4), tan((((2.0*y)/q)-1.0)*M_PI_4), 1.0, dt);
  vscl_c( 1.0 / sqrt( 1.0 + dt[0]*dt[0] + dt[1]*dt[1] ), dt, dt);

  for ( k=0; k<3; ++k ) {
    for (j=0; j<3; ++j ) {
      if ( ! GASKELLU[face][k][j] ) continue;
      v[k] = dt[j] * GASKELLU[face][k][j] * axSqrt[k];
      break;
    }
  }
  vhat_c(v,v);
  return;
}

int
main( int ac, char** av) {
double *pVec;
SpiceDouble x, y, z, u[3], v[3], ax4[4];
SpiceDouble *ax = ax4;
SpiceDouble axSqrt[3];
SpiceInt i, j, k, f;
GASKELLSHAPE* gs;

  for (i=0; i<4; ++i) {
    if ( (i+1)>=ac || 1 != sscanf(av[i+1], "%lf", ax4+i) ) {
      fprintf( stderr, "Error with argument %d (%s); exiting ...\n"
             , i+1, (i+1)<ac ? av[i+1] : "Too few arguments"
             );
      return 1;
    }
  }

  for ( i=0; i<3; ++i) axSqrt[i] = sqrt( ax[i] );

  gs = gaskellAlloc((long) ax4[3] );

  pVec = gs->verts;

  for ( f=0; f<6; ++f ) {
    for ( j=0; j<=gs->q; ++j ) {
      for ( i=0; i<=gs->q; ++i ) {
        xyf2u( gs->q, (SpiceDouble)i, (SpiceDouble)j, f, axSqrt, u);
        for ( z=k=0; k<3; ++k ) z += pow( u[k]/ax[k], 2);
        vscl_c( 1./sqrt(z), u, pVec );
        pVec += 3;
      }
    }
  }

  gaskellWrite( gs, ac<5 ? (char*)0 : av[5], 0 );

  return 0;
}
