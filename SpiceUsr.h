#ifndef __SPICEUSR_H__
#define __SPICEUSR_H__
#include <math.h>

typedef char SpiceChar;
typedef int SpiceInt;
typedef double SpiceDouble;

static void vpack_c(SpiceDouble x, SpiceDouble y, SpiceDouble z, SpiceDouble* vec3) {
  vec3[0] = x;
  vec3[1] = y;
  vec3[2] = z;
  return;
}
static void vscl_c(SpiceDouble scalar, SpiceDouble* vec3in, SpiceDouble* vec3out) {
  vec3out[0] = vec3in[0] * scalar;
  vec3out[1] = vec3in[1] * scalar;
  vec3out[2] = vec3in[2] * scalar;
  return;
}
static SpiceDouble vdot_c(SpiceDouble* vec3a, SpiceDouble* vec3b) {
  return sqrt((vec3a[0]*vec3b[0]) + (vec3a[1]*vec3b[1]) + (vec3a[2]*vec3b[2]));
}
static SpiceDouble vnorm_c(SpiceDouble* vec3) {
  return vdot_c(vec3,vec3);
}
static void vhat_c(SpiceDouble* vec3in, SpiceDouble* uvec3out) {
SpiceDouble vnorm = vnorm_c(vec3in);
  vscl_c(vnorm > 0. ? vnorm : 0.0, vec3in, uvec3out);
  return;
}
static void vadd_c(SpiceDouble* vec3a, SpiceDouble* vec3b, SpiceDouble* vec3out) {
  vpack_c( vec3a[0]+vec3b[0], vec3a[1]+vec3b[1], vec3a[2]+vec3b[2], vec3out);
  return;
}
static void vlcom_c(SpiceDouble scalarA, SpiceDouble* vec3a, SpiceDouble scalarB, SpiceDouble* vec3b, SpiceDouble* vec3out) {
SpiceDouble lclVec3[3];
  vscl_c(scalarA,vec3a,vec3out);
  vscl_c(scalarB,vec3b,lclVec3);
  vadd_c(vec3out,lclVec3,vec3out);
  return;
}
static void vlcom3_c(SpiceDouble scalarA, SpiceDouble* vec3a, SpiceDouble scalarB, SpiceDouble* vec3b, SpiceDouble scalarC, SpiceDouble* vec3c, SpiceDouble* vec3out) {
SpiceDouble lclVec3[3];
  vlcom_c(scalarA,vec3a,scalarB,vec3b,vec3out);
  vlcom_c(1.0,vec3out,scalarC,vec3c,vec3out);
  return;
}

#endif /* __SPICEUSR_H__ */

