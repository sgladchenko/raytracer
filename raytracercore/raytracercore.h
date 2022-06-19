#ifndef __RAYTRACERCORE
#define __RAYTRACERCORE

/* Just some easier way to handle buffers of arrays containing components of vectors.
In order to apply SIMD those will be usually allocated contigiously.
So technically these floats will be allocated next to each other, so we
will still be able to apply loads from memory to vector registers. */
struct __rtVector3D
{
    float x;
    float y;
    float z;
};
#define rtVector3D struct __rtVector3D

/* Just the vertices of triangular faces */
struct __rtFaces
{
    int N;
    rtVector3D *p0;
    rtVector3D *p1;
    rtVector3D *p2;
};
#define rtFaces struct __rtFaces

/* Info about direction of viewing and the point of view */
struct __rtView
{
    rtVector3D ray;
    rtVector3D Theta;
};
#define rtView struct __rtView

/* Different things that can be pre-compiled before the actual rendering */
struct __rtCache
{
    rtFaces *faces;
    /* N copies of Theta; this will be of so much use
    when applying SIMD intrinsics (N will be either 4 or 8, for 128 or 256 registers) */ 
    rtVector3D *Theta;

    /* Side edges of faces */
    rtVector3D *e1;            // e1[i] = p1[i] - p0[i]
    rtVector3D *e2;            // e2[i] = p2[i] - p0[i]
    /* Normals */
    rtVector3D *n;             // n[i] = [e1[i], e2[i]]
    /* Other cached values */
    rtVector3D *b0;           // b0[i] = p0[i] - Theta
    rtVector3D *b1;           // b1[i] = [e1[i], b0[i]]
    rtVector3D *b2;           // b2[i] = [e2[i], b0[i]]
    float *b0n;               // cn[i] = (b0,n)
};
#define rtCache struct __rtCache

/* Core functions needed for raytracing */
rtCache *rtMakeCache(const rtFaces *faces, const rtVector3D *Theta);
int rtIntersect(const rtCache *cache, float *t, float *u, float *v);
void rtRenewCache(rtCache* cache, const rtVector3D *Theta);

/* A bunch of special macros related to vector intrinsics usage */
#ifdef RT_SIMD
    // For 128-bit registers
    #define RT_VNUM 4

#else
    // Handle only one float at a time
    #define RT_VNUM 1

#endif

#endif // __RAYTRACERCORE