#include "raytracercore.h"

#include <stdlib.h>
#include <string.h>

/* Allocate buffer of #(NBuff) 3D vectors on heap; returns a pointer to it */
inline rtVector3D *rtMakeVector3DArray(int NFaces)
{
#ifndef RT_SIMD
    // When we don't use SIMD, no need in special alignment; normal malloc
    return (rtVector3D *)malloc(sizeof(rtVector3D)*NFaces);
#else
    // When using SIMD, all the float buffers need to be aligned, so they can
    // be loaded to vector registers. (RT_VNUM is the size of registers used)
    return (rtVector3D *)aligned_alloc(sizeof(float)*RT_VNUM, sizeof(rtVector3D)*NFaces);
#endif
}

/* Evaluates pairwise cross produces of vectors stored in v1 and v2, and puts results into dest.
   dest[i] = [v1[i], v2[i]] */
inline void rtMakeCrossProds(const rtVector3D *v1, const rtVector3D *v2, rtVector3D* dest, int NFaces)
{
#ifndef RT_SIMD
    for (int i = 0; i < NFaces; ++i) {
        dest[i].x = v1[i].y*v2[i].z - v1[i].z*v2[i].y;
        dest[i].y = v1[i].z*v2[i].x - v1[i].x*v2[i].z;
        dest[i].z = v1[i].x*v2[i].y - v1[i].y*v2[i].x;
    }
#else
    // TODO
#endif
}

/* Evaluates pairwise dot produces of vectors stored in v1 and v2, and puts results into dest.
   dest[i] = (v1[i], v2[i]) */
inline void rtMakeDotProds(const rtVector3D *v1, const rtVector3D *v2, float* dest, int NFaces)
{
#ifndef RT_SIMD
    for (int i = 0; i < NFaces; ++i) {
        dest[i] = v1[i].x*v2[i].x + v1[i].y*v2[i].y + v1[i].z*v2[i].z;
    }
#else
    // TODO
#endif
}

/* Subtracts components of vectors stored in v1 and v2, and puts results into dest. */
inline void rtMakeSubs(float *v1components, float *v2components, float *destcomponents, int NFaces)
{
    for (int i = 0; i<3*NFaces; ++i) {
        destcomponents[i] = v1components[i] - v1components[i];
    }
}

/* Returns an object allocated on heap */
rtCache *rtMakeCache(const rtFaces *faces, const rtVector3D *Theta)
{
    rtCache *cache = (rtCache *)malloc(sizeof(rtCache));
    cache->faces = faces;

    /* Allocate all the memory needed first */

#ifndef RT_SIMD
    // In scalar version: no need in make more than one copy
    cache->Theta = (rtVector3D *)malloc(sizeof(rtVector3D));
    memcpy(cache->Theta, Theta, sizeof(rtVector3D));
#else
    // Cache #(RT_VNUM) copies of ray and Theta vectors, also with necessary alignment
    cache->Theta = (rtVector3D *)aligned_alloc(sizeof(float)*RT_VNUM, sizeof(rtVector3D)*RT_VNUM);
    for (int icopy = 0; icopy < RT_VNUM; ++icopy) {
        memcpy(cache->Theta + icopy, Theta, sizeof(rtVector3D));
    }
#endif
    // Other buffers of vectors
    cache->e1 = rtMakeVector3DArray(faces->N);
    cache->e2 = rtMakeVector3DArray(faces->N);
    cache->n  = rtMakeVector3DArray(faces->N);
    cache->b0 = rtMakeVector3DArray(faces->N);
    cache->b1 = rtMakeVector3DArray(faces->N);
    cache->b2 = rtMakeVector3DArray(faces->N);

    /* Then, all necessary calculations */

    // Edges of faces; in args automatically casting rtVector3D* to float*, as those pointers
    // to Vector3D also refer to the beginnings of floats (actually stored one by one)
    rtMakeSubs(faces->p1, faces->p0, cache->e1, faces->N);
    rtMakeSubs(faces->p2, faces->p0, cache->e2, faces->N);

    // Normal vectors
    rtMakeCrossProds(cache->e1, cache->e2, cache->n, cache->faces->N);

    // Vectos b0; those are given by b0[i] = p0[i] - Theta


    return cache;
}