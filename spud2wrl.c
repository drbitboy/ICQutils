/* Convert Thomas SPUD model (R, Latitude, Longitude) to VRML
 *
 * Usage:
 *
 *  [VIEW=spud.shape] ./spud2wrl > spud.wrl
 *
 *  - default shape filename, if no environment variable
 *    - VIEW, or
 *    - view
 */

#include "spudshap.h"

#define nvert spudf->nv
#define np spudf->nface
#define spudf spudv->_spudf

int
main( int argc, char **argv) {
char *fnShape = (argc>1) ? argv[1] : (char *) 0;
SPUDV *spudv;
double *vXyz;
double *v0, *v1, *v2, *uvn;
VEC v01, v02, vc;
unsigned long i, iv0, iv1, iv2;

  spudv = getSpudvByname( fnShape);
  if ( !spudv) return 0;
  fprintf( stderr, "nv = %ld\n", spudf->nv);

  fprintf( stdout, "geometry IndexedFaceSet {\n  coord Coordinate { point [\n");
  for ( i=0, (vXyz = spudf->Rxyz); i<spudf->nv; ++i) {
    fprintf( stdout, "  %lf", *(vXyz++));
    fprintf( stdout, " %lf", *(vXyz++));
    fprintf( stdout, " %lf\n", *(vXyz++));
  }
  fprintf( stdout, "  ] }\n  coordIndex [\n");
  for ( i=0, (uvn=spudf->uvnorms)
      ; i<spudf->nface; ++i) {
    
    v0 = spudf->Rxyz+(3*(iv0= spudf->faceapices[i]));
    v1 = spudf->Rxyz+(3*(iv1= spudf->oe[spudf->faceoeidx[i]]));
    v2 = spudf->Rxyz+(3*(iv2= spudf->oe[spudf->faceoeidx[i]+1]));
    vsub_c( v1, v0, v01);
    vsub_c( v2, v0, v02);
    vcrss_c( v01, v02, vc);
    if ( vdot_c(vc,uvn) >= 0.0 ) {
      fprintf( stdout, "    %ld %ld %ld -1\n", iv0, iv1, iv2);
    } else {
      fprintf( stdout, "    %ld %ld %ld -1\n", iv0, iv2, iv1);
    }
    uvn += 3;
  }
  fprintf( stdout, "] }\n");

  return 0;
}
