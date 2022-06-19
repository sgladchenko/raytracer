# Raytracer

This project is mainly being done because I want to learn how to leverage capabilities that modern compilers and hardware provide today.
In particular, I want to focus on two main things:

1. **SIMD** In particular, raytracer-core is heavily based on SIMD optimizations (*TODO*), utilizing compiler vector intrinsics
2. **GPU** (May be done in future, probably using CUDA)

*TODO* Below, in a nutshell about every piece of the engine

### Agenda:

1. Make necessary infrastructure and buffers needed for both vector and scalar implementations. *(DONE)*
2. First make scalar implementation *(To be done)*

### Some thoughts:

1. Maybe inline rtMakeCrossProducts, rtMakeDotProducts etc? It might also optimize them very well, if they are used next to each other and they use SIMD intrinsics (I'm not sure)
2. When changing Theta there's no need in re-calculating edges and normals!
3. Saving pointer to rtFaces in rtCache might lead to dangling pointers...