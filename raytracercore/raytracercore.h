#ifndef RAYTRACERCORE
#define RAYTRACERCORE

/* Just some easier way to handle buffers of arrays containing components of vectors.
In order to apply SIMD those will be usually allocated contigiously. */
struct __rtVector3f
{
    float x;
    float y;
    float z;
};
#define rtVector3D struct __rtVector3f

/* Objects that stores all data about faces, including things that can be 
pre-compiled and cachec, to avoid re-calculations and increase perf. */
struct __rtFaces
{
    // Points of triangular faces
    rtVector3D* p0s;
    rtVector3D* p1s;
    rtVector3D* p2s;
    // Side edges
    rtVector3D* e1s;
    rtVector3D* e2s;
    // Normal vectors
    rtVector3D* ns;
    // Special vectors that need to be prepared
    rtVector3D* beta0s;
    rtVector3D* beta1s;
    rtVector3D* beta2s;
    // Other special values that can be pre-calculated
    // before rendering
    float* cns; // dot products of (n,beta0)
};
#define rtFaces struct __rtFaces

/* Core functions needed for raytracing */
int rtIntersect(rtFaces faces, float* t, float* u, float* v);

#endif // RAYTRACER_CORE