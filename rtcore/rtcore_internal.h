#ifndef __RTCORE_INTERNAL
#define __RTCORE_INTERNAL

/* Just some easier way to handle buffers of arrays containing components of vectors.
In order to apply SIMD these structs will be usually allocated contigiously.
So technically floats in them will be allocated next to each other, so we
will still be able to apply loads from memory to vector registers. */
struct __rtVector3D
{
    float x;
    float y;
    float z;
};
#define rtVector3D struct __rtVector3D

/* This structure defines points present in the mesh and faces spanned on them */
struct __rtMesh
{
    // Number of points
    int Npoints;
    // Number of faces
    int NFaces;
    // Points themselves
    rtVector3D *points;
    // Points of faces; e.g. p0[i] defines a base point of ith face
    int *p0, *p1, *p2;
};
#define rtMesh struct __rtMesh

/* Values describing faces of polygons */
struct __rtFaces
{
    // Total number of faces (with SIMD, must be a multiple of RT_VNUM)
    int N;
    // Base points
    rtVector3D *p;
    // Side edges
    rtVector3D *e1, *e2;
    /* Normals */
    rtVector3D *n;
    /* Unit normals */
    rtVector3D *un;
};
#define rtFaces struct __rtFaces

/* Info about direction of viewing and the point of view */
struct __rtView
{
    // Direction of view
    rtVector3D r;
    // Point of view
    rtVector3D p;
};
#define rtView struct __rtView

/* Info about lights */
struct __rtLights
{
    // Total number of point-like lights
    int N;
    // Their points
    rtVector3D *p;
};
#define rtLights struct __rtLights

/* A bunch of special macros related to vector intrinsics usage */
#ifndef RT_SIMD
    // Handle only one float at a time
    #define RT_VNUM 1
#else
    // For 128-bit registers, handle 4 floats at a time
    #define RT_VNUM 4
#endif

//////////////////////////////////////////
/* Core functions needed for raytracing */
//////////////////////////////////////////

rtFaces *rtMakeFaces(const rtMesh *mesh);
int rtIntersect(const rtFaces *faces, const rtView *view, rtVector3D* intersection);
int rtFindShadow(const rtFaces *faces, const rtVector3D *lightpoint, const rtVector3D *intersection);

#include <stdlib.h>

/* Allocate buffer of #(N) rtVector3D structs on heap; returns a pointer to it.
   When using SIMD, makes necessary alignment. */
inline rtVector3D *rtMakeVector3DArray(int N)
{
#ifndef RT_SIMD
    // When we don't use SIMD, no need in special alignment; normal malloc
    return (rtVector3D *)malloc(sizeof(rtVector3D)*N);
#else
    // When using SIMD, all the float buffers need to be aligned, so they can
    // be loaded to vector registers. (RT_VNUM is the size of registers used)
    return (rtVector3D *)aligned_alloc(sizeof(float)*RT_VNUM, sizeof(rtVector3D)*N);
#endif
}

/* Fancy, or not that much fancy, macros */

#define RT_SQR(x) (x)*(x)
// Dirty macro for dot products
#define RT_DOT(v,w) (v.x * w.x + v.y * w.y + v.z * w.z)
// Local version of zero float
#define RT_EPS 1.0e-7f
// Macro that makes comparison that a given float is close to zero
#define RT_ISZERO(x) ((x > -RT_EPS) && (x < RT_EPS))
// Macro to quickly check the range
#define RT_INRANGE(l,x,r) ((l <= x) && (x <= r))

#endif // __RTCORE_INTERNAL