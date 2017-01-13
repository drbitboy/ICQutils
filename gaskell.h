#ifndef __GASKELL_H__
#define __GASKELL_H__

// Gaskell shape construct is vertices with connectivity mapped as
// gridpoints (corners of squares) on faces of a cube
//
// - gridpoints and cube only define connectivity, not 3D positions of vertices
//
// - shape construct has power-of-two number, q, of squares per edge, q+1
//   vertices per edge
//
// - shape defined via q and cartesian coordinates of vertices
//   - edge and corner vertices are repeated
//
// - flat ASCII file format:  q, followed by 6*(q+1)*(q+1) lines with one XYZ
//   triplet per line:
//
//     q
//     X[face0][Slow0][Fast0] Y[face0][Slow0][Fast0] Z[face0][Slow0][Fast0]   ###------
//     X[face0][Slow0][Fast1] Y[face0][Slow0][Fast1] Z[face0][Slow0][Fast1]   ###   ^
//     ...                                                                    ### Face 0
//     X[face0][Slow0][Fastq] Y[face0][Slow0][Fastq] Z[face0][Slow0][Fastq]   ###___v___
//     X[face0][Slow1][Fast0] Y[face0][Slow1][Fast0] Z[face0][Slow1][Fast0]   ###   ^
//     ...                                                                    ###   Face 1

enum { GMAXQ = 512    // Maximum q, squares along edge
     , GMAXQDIM       // Maximum q+1, vertices along edge
     , GNFACES = 6    // Number of faces on cube
     };

// GASKELLU[6][3][3] (GASKELLU[FACE][j][i]) defines j (Cartesian XYZ)
// directions of i (Fast index, Slow index, and Normal-outward-to-face) for
// each face

// From triax.f:  i-coord in face frame is in j-direction with sgn u[face][j][i]

// i is index of face vertices:  i=[0,1,2] => [Fast index,Slow index,Normal-to-face and outward]

// j is cartesian index:  j=[0,1,2] => [X,Y,Z]

//                         i=0 i=1 i=2
//           u[6][j][i]      F   S   N   : Fast index; S=Slow index; Normaloutward
static const int          // |   |   |
GASKELLU[GNFACES][3][3] = // V   V   V // Ye Olde FORTRAN Equivalents:
/* FACE 0, j=X: */        {  1,  0,  0 // u(1,1,1)= 1  +X is Fast index (i=0)
/*         j=Y: */        ,  0, -1,  0 // u(2,2,1)=-1  -Y is Slow index (i=1)
/*         j=Z: */        ,  0,  0,  1 // u(3,3,1)= 1  +Z is offset of face 0 in normal direction (i=2)

/* FACE 1, j=X: */        ,  1,  0,  0 // u(1,1,2)= 1  +X is Fast index (i=0)
/*         j=Y: */        ,  0,  0, -1 // u(3,2,2)=-1  -Y is offset of face 1 in normal direction (i=2)
/*         j=Z: */        ,  0, -1,  0 // u(2,3,2)=-1  -Z is Slow index (i=1)

/* FACE 2, j=X: */        ,  0,  0, -1 // u(3,1,3)=-1  -X is offset of face 2 in normal direction (i=2)
/*         j=Y: */        , -1,  0,  0 // u(1,2,3)=-1  -Y is Fast index (i=0)
/*         j=Z: */        ,  0, -1,  0 // u(2,3,3)=-1  -Z is Slow index (i=1)

/* FACE 3, j=X: */        , -1,  0,  0 // u(1,1,4)=-1  -X is Fast index (i=0)
/*         j=Y: */        ,  0,  0,  1 // u(3,2,4)= 1  +Y is offset of face 3 in normal direction (i=2)
/*         j=Z: */        ,  0, -1,  0 // u(2,3,4)=-1  -Z is Slow index (i=1)

/* FACE 4, j=X: */        ,  0,  0,  1 // u(3,1,5)= 1  +X is offset of face 4 in normal direction (i=2)
/*         j=Y: */        ,  1,  0,  0 // u(1,2,5)= 1  +Y is Fast index (i=0)
/*         j=Z: */        ,  0, -1,  0 // u(2,3,5)=-1  -Z is Slow index (i=1)

/* FACE 5, j=X: */        ,  1,  0,  0 // u(1,1,6)= 1  +X is Fast index (i=0)
/*         j=Y: */        ,  0,  1,  0 // u(2,2,6)= 1  +Y is Slow index (i=1)
/*         j=Z: */        ,  0,  0, -1 // u(3,3,6)=-1  -Z is offset of face 5 in normal direction (i=2)
                          };

typedef struct GASKELLSHAPEstr {
  long q;
  long q1;  // q+1        2
  long vertperface;  // q1
  double *verts;      // pointer to just after struct
  double *albedos;     // pointer to just after .verts
} GASKELLSHAPE;

GASKELLSHAPE *gaskellAlloc( long q );
void gaskellFree( GASKELLSHAPE* gs );

GASKELLSHAPE *gaskellRead( char* filename );
void gaskellWrite( GASKELLSHAPE* gs, char *filename, int doAlbedos);

double *gaskellVert( GASKELLSHAPE* gs, long iFast, long iSlow, long iFace, double* albedo);
double *gaskellXYZVert( GASKELLSHAPE* gs, long iX, long iY, long iZ, double* pAlbedo);

#endif
