#include "raytracercore.h"

#include <stdio.h>
#include <stdlib.h>

int main()
{
    rtVector3D* pVecs = (rtVector3D*) aligned_alloc(sizeof(float)*4, 3*sizeof(rtVector3D));
    printf("sizeof(struct rtVector) = %lu\n", sizeof(rtVector3D));

    for (int i=0; i<5; ++i)
    {
        printf("pVecs[%d].x: %p\n", i, &(pVecs[i].x));
        printf("pVecs[%d].y: %p\n", i, &(pVecs[i].y));
        printf("pVecs[%d].z: %p\n", i, &(pVecs[i].z));
    }

    return 0;
}