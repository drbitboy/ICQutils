/* Convert Thomas SPUD model (R, Latitude, Longitude) to OOGL
 *
 * Usage:
 *
 *  [VIEW=spud.shape] ./spud2oogl
 *
 *  - default shape filename, if no environment variable
 *    - VIEW, or
 *    - view
 */

#include "spudprint.h"

int main( argc, argv) unsigned int argc; char **argv; {
SPUDR		spudr;
SPUDF		spudf;

  spudr.nlatR = SPUDRNLAT;
  spudr.nlonR = SPUDRNLON;
  spudr.eastlon = getview( spudr.Rmodel[0], &spudr.nlatR, &spudr.nlonR); /**/
  if ( spudr.eastlon == -1) {
    fprintf( stderr, "Failed to load shape model\n");
    return(0);
  }
  if ( !spudr.eastlon) spudr.eastlon = -1;

  printf( "#eastlon,nlat,nlon=%d %ld %ld\n", spudr.eastlon, spudr.nlatR
		, spudr.nlonR), fflush(stdout);

  rmod2face( &spudr, &spudf);

  spudprint_oogl( &spudf);

  return 0;

}
