#!/usr/bin/gawk -f

### check_positive_normals.gawk
###
### Check plate normals of well-behaved STL model
###
### Well-behaved:  all surface normals point outward from the origin,
###                a la Peter Thomas SPUD models, with any ray from the
###                origin never intersecting the surface in more than
###                one place.
###
### 1) dot products of facet normal with
###
###      Vertex0,
###      Vertex1,
###      Vertex2,
###
###    appended to end of [vertex X Y Z] lines, should be positive
###
### 2) Scaled dot products of facet normal with
###
###      VCROSS(Vertex1-Vertex0, Vertex2-Vertex0),
###
###    printed out after last [vertex X Y Z] line for each facet,
###    should also be positive and equal to +1.0
###     

function vdot(x0,y0,z0,x1,y1,z1) { return x0*x1 + y0*y1 + z0*z1 }
function vnorm(x0,y0,z0) { return sqrt(vdot(x0,y0,z0,x0,y0,z0)) }

$1=="facet" { nx=$3+0.; ny=$4+0.; nz=$5+0. ; vtx=0 ; print FNR, $0; next }

$1=="vertex" {
  vx[vtx] = $2
  vy[vtx] = $3
  vz[vtx] = $4
  print FNR,$0,vdot(nx,ny,nz,$2,$3,$4)

  # Difference from v?[0] to either v?[1] or v>[2]
  if (vtx>0) {
    vx[vtx] -= vx[0]
    vy[vtx] -= vy[0]
    vz[vtx] -= vz[0]
  }
  if (++vtx<3) next

  # Cross product
  x = vy[1] * vz[2] - vz[1]*vy[2]   # x <= y cross z
  y = vz[1] * vx[2] - vx[1]*vz[2]   # y <= z cross x
  z = vx[1] * vy[2] - vy[1]*vx[2]   # z <= x cross y
  print vdot(x,y,z,nx,ny,nz) / (vnorm(x,y,z)*vnorm(nx,ny,nz))
}
