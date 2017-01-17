/* Convert Thomas SPUD model (R, Latitude, Longitude) to X plate format
 *
 * Usage:
 *
 *  [VIEW=spud.shape] ./spud2xplate > spud.xplt
 *
 *  - default shape filename, if no environment variable
 *    - VIEW, or
 *    - view
 */
#include <stdio.h>
#include "spudprint.h"

int main( argc, argv) unsigned int argc; char **argv; {
SPUDV *spudv;
char *fnShape = (argc > 1) ? argv[1] : (char *) 0;

  if ( !(spudv = getSpudvByname( fnShape)) ) return 0;
  spudprint_xplate( spudv->_spudf);
  return 0;
}
