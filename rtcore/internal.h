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

/* Fancy, or not that much fancy, macros */

#define RT_SQR(x) (x)*(x)
// Dirty macro for dot products
#define RT_DOT(v,w) ((v).x*(w).x + (v).y*(w).y + (v).z*(w).z)
// Local version of zero float
#define RT_EPS 1.192093e-07
// Macro that makes comparison that a given float is close to zero
#define RT_ISZERO(x) ((x > -RT_EPS) && (x < RT_EPS))
// Macro to quickly check the range
#define RT_INRANGE(l,x,r) ((l <= x) && (x <= r))

#endif // __RTCORE_INTERNAL