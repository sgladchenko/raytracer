#include "raytracercore.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{

    // Check if floats are allocated next to each other physically & check their alignment

    rtVector3D *pVecs = (rtVector3D *)aligned_alloc(sizeof(float)*4, sizeof(rtVector3D)*5);
    printf("sizeof(struct rtVector) = %lu\n", sizeof(rtVector3D));

    for (int i=0; i<5; ++i)
    {
        printf("&pVecs[%d].x: %p\n", i, &(pVecs[i].x));
        printf("&pVecs[%d].y: %p\n", i, &(pVecs[i].y));
        printf("&pVecs[%d].z: %p\n", i, &(pVecs[i].z));
    }
    printf("\n");

    // Check if it's possible to use memcpy for copying structures

    rtVector3D s1 = {.x=1.0, .y=2.0, .z=3.0};
    rtVector3D *ps2;

    ps2 = (rtVector3D *)aligned_alloc(sizeof(float)*4, sizeof(rtVector3D));
    memcpy(ps2, &s1, sizeof(rtVector3D));

    printf("ps2->x: %f\n", ps2->x);
    printf("ps2->y: %f\n", ps2->y);
    printf("ps2->z: %f\n\n", ps2->z);

    // Pointer to the beginning of the structs and pointer to the first float, are they same?

    float *pFloats = (float *)pVecs;
    printf("pVecs: %p\n", pVecs);
    printf("pFloats = (float *)pVecs: %p\n", pFloats);

    float* ps2_floats = (float *)ps2;
    printf("ps2_floats = (float *)ps2\n");
    printf("ps2_floats[0]=%f, ps2_floats[1]=%f, ps2_floats[2]=%f\n", ps2_floats[0], ps2_floats[1], ps2_floats[2]);

    return 0;
}