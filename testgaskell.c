// cc -o testgaskell -I.../cspice/include gaskelltriax.c .../cspice/lib/cspice.a -lm
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SpiceUsr.h"
#include "gaskell.h"

const char Usage[] = { "\n\
Usage:  testgaskell [shaper.txt] > shapercopy.txt\n\
e.g.\n\
\n\
  ./gaskelltriax 3 2 1 2 | tee x.x | ./testgaskell > y.y\n\
\n\
  diff -y x.x y.y\n\n" };


int
main( int ac, char** av) {
char *filename = ac>1 ? av[1] : 0;
GASKELLSHAPE* gs = gaskellRead( filename );
long iFace, iFast, iSlow;
SpiceDouble xFace, xFast, xSlow;
long i, j;
SpiceDouble vOnes[3];

  if ( !gs && filename && !strcmp("--help",av[1]) ) {
    fprintf( stderr, "%s", Usage );
    return 0;
  }

  //gaskellWrite( gs, (char *)0, 0);
  fprintf( stdout, "%12ld\n", gs->q);

  vpack_c( 1,1,1,vOnes );

  for ( iFace=0; iFace<GNFACES; ++iFace ) {
  SpiceDouble dvFSN[3][3];  // mFSN[0], [1], [2] are directions of iFast, iSlow, iFace
  SpiceDouble bvFSN[3];     // base position of iFast, iSlow, iFace
  SpiceDouble dq = gs->q;

    vpack_c( 0,0,0, bvFSN);

    for (i=0; i<3; ++i) {
    double factor;
      vpack_c( GASKELLU[iFace][0][i], GASKELLU[iFace][1][i], GASKELLU[iFace][2][i], dvFSN[i] );
      factor = vdot_c(vOnes,dvFSN[i]) < 0 ? (i<2?-dq:0) : (i<2?0:dq);
      vlcom_c( factor, dvFSN[i], 1.0, bvFSN, bvFSN );
      //printf( "iFace=%ld, i=%ld, dvFSN=[%4.1lf %4.1lf %4.1lf]\n", iFace, i, dvFSN[i][0], dvFSN[i][1], dvFSN[i][2] );
    }

    //printf( "iFace=%ld, bvFSN=[%4.1lf %4.1lf %4.1lf]\n", iFace, bvFSN[0], bvFSN[1], bvFSN[2] );

    for ( iSlow=0; iSlow < gs->q1; ++iSlow ) {
    SpiceDouble vFSN[3];
      if ( iSlow==0 && iFace>0 ) continue;
      if ( iSlow==gs->q && iFace==(GNFACES-1) ) continue;

      vlcom3_c( 1.0, bvFSN, iSlow, dvFSN[1], -1.0, dvFSN[0], vFSN);

      for ( iFast=0; iFast < gs->q1; ++iFast ) {
      SpiceDouble* vXYZ;

        vlcom_c( 1.0, vFSN, 1.0, dvFSN[0], vFSN);

        if ( iFast==gs->q && iFace>0 ) continue;
        if ( iFast==0 && iFace==(GNFACES-1) ) continue;

        //printf( "iFast=%ld; iSlow=%ld; iFace=%ld, vFSN=[%4.1lf %4.1lf %4.1lf]\n", iFast, iSlow, iFace, vFSN[0], vFSN[1], vFSN[2] );

        vXYZ = gaskellXYZVert( gs, (long) vFSN[0], (long) vFSN[1], (long) vFSN[2], (double*) 0);

        for (i=0; i<3; ++i) fprintf( stdout, "%12.5lf", vXYZ[i] );
        //fprintf( stdout, "%12ld %1ld", ((long)(vXYZ - gs->verts)) / 3L, ((long)(vXYZ - gs->verts)) % 3L );
        fprintf( stdout, "%s", "\n");
      }
    }
  }

  return 0;
}
