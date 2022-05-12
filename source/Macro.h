#ifndef __MACRO
#define __MACRO

#define NOSIMD 0
#define AVX2   1

///////////////////////////////////////////////////////////////////////////////
// Here: just the configuration of the library
// 1. Allow/disallow SIMD instructions and specifiy what type of instructions
//    we want to use.
      #define RT_SIMD AVX2
// 2. Allow using uninialized reads in some special circumstances to avoid 
//    additional checks, make as much noexcept as possible,
//    i.e. improve performance.
      //#define RT_UREADS
///////////////////////////////////////////////////////////////////////////////

#if RT_SIMD == AVX2

    // Registers with 256 bits, i.e. 4 doubles
    #include "immintrin.h"
    #define RT_3DARRAY(T,name) alignas(256) T name[4]

#else

    // Just don't used them; instead use normal loops
    #define RT_3DARRAY(T,name) T name[3]

#endif

#endif // __MACRO