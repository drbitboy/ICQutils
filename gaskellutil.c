/* Gaskell ICQ shape model utilities */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gaskell.h"
/*
typedef struct GASKELLSHAPEstr {
  long q;
  long q1;  // q+1        2
  long vertperface;  // q1
  double *verts;     // pointer to just after struct
  double *albedos;
} GASKELLSHAPE;
*/

GASKELLSHAPE *gaskellAlloc( long q ) {
long dblsiz = sizeof(double);
long q1 = q+1;
long vpf = q1*q1;
long vertmem = GNFACES * vpf * 3 * dblsiz;
long albedomem = GNFACES * vpf * dblsiz;
GASKELLSHAPE *gs = (GASKELLSHAPE*) malloc( sizeof(GASKELLSHAPE) + albedomem + vertmem);
  gs->q = q;
  gs->q1 = q1;
  gs->vertperface = vpf;
  gs->albedos = (double*) (gs+1);
  gs->verts = gs->albedos + (GNFACES * vpf);   // if this changes, change gaskellRead() loop below
  memset( gs->albedos, 0, vertmem + albedomem);
  return gs;
}

void gaskellFree( GASKELLSHAPE* gs ) {
  free( gs);
  return;
}

GASKELLSHAPE *gaskellRead( char* filename ) {
FILE* fstrm = filename ? (*filename ? (strcmp("-",filename) ? fopen(filename,"r") : stdin) : stdin) : stdin;
char c1k[1024];
long q;
GASKELLSHAPE *gs = 0;
double *pVert;
double *pAlbedo;
long ssc;

# define RTNERR { if (gs) gaskellFree(gs); if (fstrm && fstrm != stdin) fclose(fstrm); return 0; }
  if ( !fstrm ) RTNERR
  if ( !fgets(c1k,1023,fstrm) ) RTNERR
  if ( 1 != sscanf( c1k, "%ld", &q) ) RTNERR
  gs = gaskellAlloc(q);

  pVert = gs->verts;
  // This loop depends on gs->verts following, and being contiguous with, gs->albedos
  for ( pAlbedo=gs->albedos; pAlbedo<gs->verts; ++pAlbedo, pVert+=3 ) {
    if ( !fgets(c1k,1023,fstrm) ) RTNERR
    ssc = sscanf( c1k, "%lf %lf %lf %lf", pVert, pVert+1, pVert+2, pAlbedo);
    if ( ssc < 3 ) RTNERR
    if ( ssc < 4 ) *pAlbedo = -1.0;
  }
  if (fstrm != stdin) fclose(fstrm);
# undef RTNERR

  return gs;
}

////////////////////////////////////////////////////////////////////////
void
gaskellWrite( GASKELLSHAPE* gs, char *filename, int doAlbedos) {
double *pVert;
double  *pAlbedo;
FILE *pF = filename ? *filename ? strcmp(filename,"-") ? fopen( filename, "w" ) : stdout : stdout : stdout;

  fprintf( pF, "%12ld\n", gs->q );

  pVert = gs->verts;
  // This loop depends on gs->verts following, and being contiguous with, gs->albedos
  for ( pAlbedo=gs->albedos; pAlbedo<gs->verts; ++pAlbedo ) {
  int k;
    for (k=0; k<3; ++k) {
    double v = *(pVert++);
      fprintf( pF, "%12.5lf", v==-0.0 ? 0.0 : v );
    }
    if ( doAlbedos ) fprintf( pF, "%12.5f", *pAlbedo );
    fprintf( pF, "%s", "\n" );
  }
  if (filename) fclose( pF );
  return;
}

////////////////////////////////////////////////////////////////////////
// Return optional albedo and pointer to vertex coordinate vector given
// the three Gaskell shape model indices:  face; fast; slow.

double *gaskellVert( GASKELLSHAPE* gs, long iFast, long iSlow, long iFace, double* pAlbedo) {
long offset = (iFace * gs->vertperface) + (iSlow * gs->q1) + iFast;
  if ( pAlbedo ) *pAlbedo = gs->albedos[offset];
  return gs->verts + (3 * offset);
}

////////////////////////////////////////////////////////////////////////
// Return optional albedo and pointer to vertex coordinate vector given
// three Cartesian indices to the cube:  iX; iY; iZ.
// ***N.B. All indices should be in the range 0 to q, inclusive, and at
//         least one of the cartesian indices should be 0 or q, but no
//         check is made for these conditions

double *gaskellXYZVert( GASKELLSHAPE* gs, long iX, long iY, long iZ, double* pAlbedo) {
long iFast, iSlow, iFace, j, i;
    
  // Issue is which face owns which edges:
  // Top Face 0 (iZ==q) owns all four of its edges
  // Side Faces 1,2,3,4 ((iX or iY)==(0 or q)) each own two edges
  // - The bottom edge (iZ==0), parallel to the iFast direction
  // - The iFast==0 edge, parallel to iZ and iSlow direction
  // Bottom Face 5 (iZ==0) owns none of its edges

  if (iZ<gs->q) {    // Not top face

    if ( iZ || !(iX%gs->q) || !(iY%gs->q) ) {  // 0<iZ<q, or (iX or iY) on a side face (1,2,3,4)

      iFace = iX                                          //  iX>0:  Face 1, 3, or 4; not Face 2 (iX==0)
                 ? ( iX < gs->q ? ( iY         ? 3 : 1)   //  iX<q:  not Face 4 (iX==q); either Face 3 (iY==q) or 1 (iY==0)
                                : ( iY < gs->q ? 4 : 3)   // iX==q:  either Face 4 (iX==q), or iFast edge of Face 3 (iY==q)
                   )
                  : (iY ? 2 : 1);                         // iX==0:  either Face 2 (iX==0), or iFast=0 edge of Face 1 (iY==0)

    } else {     // (iZ==0 && 0<iX<q && 0<y<q) =>> bottom face
      iFace = 5;
    }
  } else {       // iZ==q => top face
    iFace = 0;
  }

  // Convert iFace, iX, iY, and iZ to iFast and iSlow
# define PICKiXYZ (j<1 ? iX : (j<2 ? iY : iZ))
  for ( i=0; i<2; ++i ) {
  long* pLong = i ? &iSlow : &iFast;
    for ( j=0; j<3; ++j) {
      if      ( GASKELLU[iFace][j][i] ==  1 ) { *pLong = PICKiXYZ;         break; }
      else if ( GASKELLU[iFace][j][i] == -1 ) { *pLong = gs->q - PICKiXYZ; break; }
    }
  }
# undef PICKiXYZ

  // Return pointer to vertex and optional albedo, using iFast, iSlow and iFace
  return  gaskellVert( gs, iFast, iSlow, iFace, pAlbedo);
}
