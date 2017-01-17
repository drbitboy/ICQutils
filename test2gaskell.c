/* test2gaskell.c
 *
 * Write out unique vertices of Gaskell ICQ shape model, starting at the
 * center of the bottom face of the Gaskell Cube-Cartesian System (CCS),
 * then spiraling around that center out to the edges of the cube bottom
 * face, then around and up the cube side faces, then in to the center of
 * the cube top face.
 *
 * Precede that by a line with the unique vertex count and triangular plate
 * count; follow it by the vertex indices for each triangular plate
 *
 * Compile:
 *
 *   cc -o test2gaskell test2gaskell.c gaskellutil.c
 *
 * Usage:
 *   ./test2gaskell < gaskell.icq > gaskell.plt
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gaskell.h"

const char Usage[] = { "\n\
Usage:  [T2G_PRINTEXTRAS=yes] test2gaskell [shaper.txt] > shaperunique.txt\n\
e.g.\n\
\n\
  ./gaskelltriax 3 2 1 2 | ./test2gaskell > y.y\n\n" };

int
main( int ac, char** av) {

long np, nv, hq;  // Number of triangular plates, number of vertices, q/2 (half q)
long ix, iy;      // X and Y coordinates, in CCS, of points to write
long ixy0;        // ix=iy=ixy0, starting point of each loop around cube
long iz0;         // Vertical CCS positions
long i, j, i4;    // counters
long n;           // size of edge of loop around cube
long iv;          // zero-based index of current vertex

double **vertOuts;
double **vertOut;
long *iverts, *ivertEnd, *ivert;

char *filename = ac>1 ? av[1] : 0;              // Read Gaskell shape model
GASKELLSHAPE* gs = gaskellRead( filename );

char *printExtras = getenv("T2G_PRINTEXTRAS");  // debugging

  if ( !gs && filename && !strcmp("--help",av[1]) ) {
    fprintf( stderr, "%s", Usage );
    return 0;
  }

  // Number of triangular plates = twice (12 = 2 * 6) number of sub-faces
  // on each cube face

  np = gs->q * gs->q * 12;
  nv = (np >> 1) + 2;       // invert np = 2 * nv - 4

  // Pointers to pointer to output vertices (not used yet 2012-11-29)
  vertOut = vertOuts = (double**) malloc( nv * sizeof(double*) );
  ivertEnd = iverts = (long*) malloc( 3 * np * sizeof(long) );

  hq = gs->q >> 1;          // half-q i.e. q/2

  // Write vertices of cube along paths defined by succession of
  // square paths:
  //
  // - Center vertex of Cube Bottom Face 5 (iz0==0)
  //
  // - Outward around center vertex of Bottom Face 5 (iz0==0),
  //   from [x,y]=[q/2-1,q/2-1] to [0,0]
  //
  // - Up (iz = 1 to q) sides of cube at [x,y]=[0,0]
  //
  // - Inward toward center vertex of Top Face 0 (iz0==q),
  //   from [x,y]=[1,1] to [q/2-1,q/2-1]
  //
  // - Center vertex of Cube Top Face 0 (iz==q)

  fprintf( stdout, "%12ld%12ld\n", nv, np );   // header for plate format

  // n holds the length of the sides of the squares; n==0 implies a center
  // vertex on the Cube Top or Bottom Face; side lengths are always even,
  // but n==1 is a special case for the writing the center vertex of bottom
  // face on the first pass

  n = 1;
  iv = 0;

  // Loop terminates after center vertex (n==0) on Cube Top Face 0 (iz0==q)
  while ( iz0 < gs->q || n > 0) {
  long oes[9];                    // "other ends" to define triangular plates
  long *oesEnd;
  long *oe;
  long n4;      //= 4 * n = n << 2;
  long n8;      //= 8 * n = n << 3 = n4 < 1;

    if ( n==1 ) {  // Special case for n==1:  center of Bottom Face 5

      // start at center of Cube Bottom Face 5

      iz0 = 0;    // Bottom Face 5
      ixy0 = hq;  // [ix,iy]=[q/2,q/2] => center of Bottom Face 5 (iz0==0)
      n = 0;      // will write one vertex; this also resets n for next loop

    } else if ( !iz0 && ixy0>0 ) {  // !iz0 => Bottom Face 5
                                    // ixy0>0 => square is not yet to edge
      // Move out (ix=iy=--ixy0) from center toward edges of Bottom Face 5; iz0=0
      n += 2;  // Expand square size
      --ixy0;  // Shift starting corner of square along diagonal from
               // Center toward [ix,iy]=[0,0]

    } else if ( iz0 < gs->q ) {  // iz0<q => not to Top Face 0 yet
      // Move up (++iz0) Side Faces 1, 2, 3 and 4, stopping before the top edge
      ++iz0;     // Move up one
      ixy0 = 0;  // Starting corner is at [ix,iy]=[0,0] (redundant if q even)

    } else {   // iz0==q => on Top Face 0
      // Move in (ix=iy=++ixy0) from edges toward center of Top Face 0; iz0==q
      n -= 2;  // Reduce square size
      ++ixy0;  // Shift starting corner of square along diagonal
               //   from [ix,iy]=[1,1] toward center
    }

    n4 = n << 2;
    n8 = n4 << 1;

    // Get and write a square, in CCS, of vertices given ix, iy, iz0 and n

    ix = iy = ixy0;  // Start corner is along line between [q/2,q/2] and [0,0]

    for ( i4=0; i4<(n?4:1); ++i4) {  // loop over four sides of square, or once if n==0

    // x and y deltas for loop of square, in CCS, index is i4:
    static const long dixs[4] = { 0, 1, 0, -1 };
    static const long diys[4] = { 1, 0, -1, 0 };
    long dix = dixs[i4];
    long diy = diys[i4];

      for ( i=0; i < (n?n:1); ++i, ++iv ) {  // n verts per side, but at least 1

        // Get pointer to vertex from Gaskell shape structure,
        // print out vertex coordinates and optional debuggin data,
        // increment pointer to pointer for next vertex
        // step to next vertex [ix,iy] along side

        *vertOut = gaskellXYZVert( gs, ix, iy, iz0, 0 );
        fprintf( stdout, "%12.5lf%12.5lf%12.5lf", vertOut[0][0], vertOut[0][1], vertOut[0][2]);
        if ( printExtras ) fprintf( stdout, "%5ld%5ld%5ld%5ld", ((long)(vertOut[0] - gs->verts)/3), ix, iy, iz0);
        fprintf( stdout, "\n" );

        ++vertOut;

        ix += dix;          // step to next vertex along side
        iy += diy;          //  "

        // triangular plate connectivity:

        oesEnd = oes;

        // Cube Bottom Face 5
        if ( !iz0 && n < gs->q ) {
          if ( !n ) {                    // - center
          int ii;
            *(oesEnd++) = iv + 8;        //   - add vertex 8
            for (ii=1; ii<9; ++ii) {     //   - add vertices 1-8
              *(oesEnd++) = iv + ii;
            }
          } else {                       // - not center
            if ( !i ) {                  //   - corners
              if ( !i4 ) {               //   - first corner
                *(oesEnd++) = iv + n4 - 1;
                *(oesEnd++) = iv + n8 + 6;
              }
              *(oesEnd++) = iv + (i4 ? (n4 + 2*i4 - 1) : (n8 + 7));  // - any corner
              *(oesEnd++) = iv + n4 + 2*i4;
            }
            if ( i4!=3 || i!=(n-1) ) {   // - not last vertex in square path
              *(oesEnd++) = iv + n4 + 2*i4 + 1;
              *(oesEnd++) = iv + n4 + 2*i4 + 2;
              *(oesEnd++) = iv + 1;
            }
          }

        // Cube Side Faces 1-4
        } else if  ( iz0 < gs->q ) {

          if ( !i && !i4 ) {             // - first corner
            *(oesEnd++) = iv + n4 - 1;
            *(oesEnd++) = iv + n8 - 1;
          }
          if ( i4<3 || i!=(n-1) ) {     // - not last vertex in square path
            *(oesEnd++) = iv + n4;
            *(oesEnd++) = iv + n4 + 1;
            *(oesEnd++) = iv + 1;
          }

        // Cube Top Face 0
        } else if ( n ) {
          // - skip all corners except first
          // - skip last vertex in square path (i==(n-1) && i4==3)
          if ( (i || !i4) && (i!=(n-1) || i4<3) ) {

            if ( !i ) {                 // - first corner (i4==0)
              *(oesEnd++) = iv + n4 - 1;
              *(oesEnd++) = iv + n4;
              *(oesEnd++) = iv + 1;

            } else if ( i==(n-1)        // - last vertex before next i4 & corner
                     || (i==(n-2) && i4>2) ) {    // - or last vertex of path
              *(oesEnd++) = iv + n4 - ((2*i4) + 1);
              *(oesEnd++) = iv + 2;
              *(oesEnd++) = iv + 1;

            } else {                    // anything else
              *(oesEnd++) = iv + n4 - ((2*i4) + 1);
              *(oesEnd++) = iv + n4 - (2*i4);
              *(oesEnd++) = iv + 1;
            }
          }
        }
        for ( oe=oes+1; oe<oesEnd; ++oe, ivertEnd+=3 ) {
          ivertEnd[0] = iv;
          ivertEnd[1] = oe[-1];
          ivertEnd[2] = oe[0];
        }
      } // for i=0 ...
    } // for i4=0 ...
  } // while ( iz0 < q || n > 0 )

  for ( ivert=iverts; ivert<ivertEnd; ) {
  int ii;
    for (ii=0; ii<3; ++ii) fprintf( stdout, "%12ld", *(ivert++) );
    fprintf( stdout, "\n" );
  }
  gaskellFree(gs);
  free( vertOuts );
  free( iverts );

  return 0;
}
