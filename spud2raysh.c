/* Convert Thomas SPUD model (R, Latitude, Longitude) to rayshade format
 *
 * Usage:
 *
 *  [VIEW=spud.shape] ./spud2raysh
 *
 *  - default shape filename, if no environment variable
 *    - VIEW, or
 *    - view
 */

#include "spudprint.h"

int main( argc, argv) unsigned int argc; char **argv; {
SPUDR           spudr;
SPUDF           spudf;

  spudr.nlatR = SPUDRNLAT;
  spudr.nlonR = SPUDRNLON;
  spudr.eastlon = getview( spudr.Rmodel[0], &spudr.nlatR, &spudr.nlonR); /**/
  if ( spudr.eastlon == -1) {
    fprintf( stderr, "Failed to load shape model\n");
    return(0);
  }
  if ( !spudr.eastlon) spudr.eastlon = -1;
  spudr.latdel = 180.0 / (spudr.nlatR-1);
  spudr.londel = 360.0 / (spudr.nlonR-1);

  rmod2face( &spudr, &spudf);

  spudprint_raysh( &spudr, &spudf);

  return 0;
}
