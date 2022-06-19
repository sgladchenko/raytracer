#include "rtcore_internal.h"

#include <string.h>
#include <math.h>

/* Assembles rtFaces structure on heap using info provided in rtMeshes;
   For the actual faces evaluates their edges, normals etc */
rtFaces *rtMakeFaces(const rtMesh *mesh)
{
    // Allocate it on heap, so it'll be easier to pass it to the other functions
    rtFaces *faces = (rtFaces *)malloc(sizeof(rtFaces));
    faces->N = mesh->NFaces;

    // Allocate memory needed for contents of rtFaces
    faces->p  = rtMakeVector3DArray(faces->N);
    faces->e1 = rtMakeVector3DArray(faces->N);
    faces->e2 = rtMakeVector3DArray(faces->N);
    faces->n  = rtMakeVector3DArray(faces->N);
    faces->un = rtMakeVector3DArray(faces->N);

    // Just make a copy of base points in rtFaces
    memcpy(faces->p, mesh->p0, sizeof(rtVector3D)*faces->N);

    // Side edges of faces; e1 = p1 - p0 and e2 = p2 - p0
    for (int i = 0; i < mesh->NFaces; ++i)
    {
        faces->e1[i].x = mesh->points[mesh->p1[i]].x - mesh->points[mesh->p0[i]].x;
        faces->e1[i].y = mesh->points[mesh->p1[i]].y - mesh->points[mesh->p0[i]].y;
        faces->e1[i].z = mesh->points[mesh->p1[i]].z - mesh->points[mesh->p0[i]].z;

        faces->e2[i].x = mesh->points[mesh->p2[i]].x - mesh->points[mesh->p0[i]].x;
        faces->e2[i].y = mesh->points[mesh->p2[i]].y - mesh->points[mesh->p0[i]].y;
        faces->e2[i].z = mesh->points[mesh->p2[i]].z - mesh->points[mesh->p0[i]].z;
    }

    // Normals; just cross products [e1, e2]
    for (int i = 0; i < mesh->NFaces; ++i)
    {
        faces->n[i].x = faces->e1[i].y*faces->e2[i].z - faces->e1[i].z*faces->e2[i].y;
        faces->n[i].y = faces->e1[i].z*faces->e2[i].x - faces->e1[i].x*faces->e2[i].z;
        faces->n[i].z = faces->e1[i].x*faces->e2[i].y - faces->e1[i].y*faces->e2[i].x;

        // Also stash their unit versions
        float invnorm = 1.0f / sqrtf(RT_SQR(faces->n[i].x) + RT_SQR(faces->n[i].y) + RT_SQR(faces->n[i].z));
        faces->un[i].x = faces->n[i].x * invnorm;
        faces->un[i].y = faces->n[i].y * invnorm;
        faces->un[i].z = faces->n[i].z * invnorm;
    }

    return faces;
}

/* Comments:

I'm not sure whether in this module we would ever need SIMD; internally when allocating memory, for allowance SIMD in further
it makes alignment, but for the calculations themselves there's no need in SIMD, as this all is done only once
while loading faces to the engine. */