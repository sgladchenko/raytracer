#ifndef __RTCORE_INTERNAL
#define __RTCORE_INTERNAL

#include "rtcore.h"

/* A bunch of special macros related to vector intrinsics usage */
#ifndef RT_SIMD
    // Handle only one float at a time
    #define RT_VNUM 1
#else
    // For 128-bit registers, handle 4 floats at a time
    #define RT_VNUM 4
#endif

#include <stdlib.h>

/* Allocate buffer of #(N) rtVector3D structs on heap; returns a pointer to it.
   When using SIMD, makes necessary alignment. */
inline struct rtVector3D *rtMakeVector3DArray(int N)
{
#ifndef RT_SIMD
    // When we don't use SIMD, no need in special alignment; normal malloc
    return (struct rtVector3D *)malloc(sizeof(struct rtVector3D)*N);
#else
    // When using SIMD, all the float buffers need to be aligned, so they can
    // be loaded to vector registers. (RT_VNUM is the size of registers used)
    return (struct rtVector3D *)aligned_alloc(sizeof(float)*RT_VNUM, sizeof(struct rtVector3D)*N);
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