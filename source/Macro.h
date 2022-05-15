#ifndef __MACRO
#define __MACRO

#define NOSIMD 0
#define AVX2   1

// Allow using uninialized reads in some special circumstances to avoid 
// additional checks, make as much noexcept as possible,
// i.e. improve performance.
#define RT_UREADS_UNITVECTOR

#define RT_3DARRAY_SIZE 3
#define RT_3DARRAY(T,name) T name[RT_3DARRAY_SIZE]

#define RT_SQR(x) (x)*(x)

// In some really special cases explicit SIMD instructions might become needed
#define RT_SIMD AVX2

#endif // __MACRO