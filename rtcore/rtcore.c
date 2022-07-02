#include "internal.h"

#include <string.h>
#include <math.h>

/* Allocate buffer of #N rtVector3D structs on heap; returns a pointer to it.
   When using SIMD, makes necessary alignment. */
struct rtVector3D *rtMakeVector3DArray(int N)
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

/* Allocates a new rtMesh instance and the memory needed to store all 
  its contents */
struct rtMesh  *rtMakeMesh(int Npoints, int Nfaces)
{
    struct rtMesh *mesh = (struct rtMesh *)malloc(sizeof(struct rtMesh));
    mesh->Npoints = Npoints;
    mesh->Nfaces  = Nfaces;

    // Here: no need in alignment, as this buffer isn't going to be used
    // in the actual raytracing
    mesh->points = (struct rtVector3D *)malloc(sizeof(struct rtVector3D) * Npoints);
    mesh->p0 = (int *)malloc(sizeof(int) * Npoints);
    mesh->p1 = (int *)malloc(sizeof(int) * Npoints);
    mesh->p2 = (int *)malloc(sizeof(int) * Npoints);

    return mesh;
}

/* Assembles rtFaces structure on heap using info provided in rtMeshes;
   For the actual faces evaluates their edges, normals etc */
struct rtFaces *rtMakeFaces(struct rtMesh *mesh)
{
    // Allocate it on heap, so it'll be easier to pass it to the other functions
    struct rtFaces *faces = (struct rtFaces *)malloc(sizeof(struct rtFaces));
    faces->N = mesh->Nfaces;

    // Allocate memory needed for contents of rtFaces
    faces->p  = rtMakeVector3DArray(faces->N);
    faces->e1 = rtMakeVector3DArray(faces->N);
    faces->e2 = rtMakeVector3DArray(faces->N);
    faces->n  = rtMakeVector3DArray(faces->N);
    faces->un = rtMakeVector3DArray(faces->N);

    // Just make a copy of base points in rtFaces
    memcpy(faces->p, mesh->p0, sizeof(struct rtVector3D)*faces->N);

    // Side edges of faces; e1 = p1 - p0 and e2 = p2 - p0
    for (int i = 0; i < mesh->Nfaces; ++i)
    {
        faces->e1[i].x = mesh->points[mesh->p1[i]].x - mesh->points[mesh->p0[i]].x;
        faces->e1[i].y = mesh->points[mesh->p1[i]].y - mesh->points[mesh->p0[i]].y;
        faces->e1[i].z = mesh->points[mesh->p1[i]].z - mesh->points[mesh->p0[i]].z;

        faces->e2[i].x = mesh->points[mesh->p2[i]].x - mesh->points[mesh->p0[i]].x;
        faces->e2[i].y = mesh->points[mesh->p2[i]].y - mesh->points[mesh->p0[i]].y;
        faces->e2[i].z = mesh->points[mesh->p2[i]].z - mesh->points[mesh->p0[i]].z;
    }

    // Normals; just cross products [e1, e2]
    for (int i = 0; i < mesh->Nfaces; ++i)
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

/* Allocate rtLights and its contents on heap */
struct rtLights *rtMakeLights(int Nlights)
{
    struct rtLights *lights = (struct rtLights *)malloc(sizeof(struct rtLights));
    lights->N = Nlights;
    lights->p = (struct rtVector3D *)malloc(sizeof(struct rtVector3D)*Nlights);

    return lights;
}

/* Free all the contents of rtMesh and the object itself */ 
void rtFreeMesh(struct rtMesh *mesh)
{
    free(mesh->points);
    free(mesh->p0); free(mesh->p1); free(mesh->p2);
    free(mesh);
}

/* The function which just frees all the buffers allocated in rtFaces and the structure itself */
void rtFreeFaces(struct rtFaces *faces)
{
    free(faces->p); free(faces->e1); free(faces->e2);
    free(faces->n); free(faces->un);
    free(faces);
}

/* Free all the contents of rtLights and the object itself */ 
void rtFreeLights(struct rtLights *lights)
{
    free(lights->p);
    free(lights);
}

/* Returns index of the face appeared to be closest one to the point of view in the direction
   of viewing (t>0); returns -1 if there's no face intersected. */
int rtIntersect(struct rtFaces *faces, struct rtView *view, struct rtVector3D *intersection)
{
#ifndef RT_SIMD
   // Provide some invalid values, just to avoid any uninitialized reads
   float tcur, tmin, ucur, umin, vcur, vmin;
   tcur = tmin = INFINITY;
   ucur = umin = vcur = vmin = -1.0f;
   int imin = -1; // Will be returned if no intersections found

   for (int i = 0; i < faces->N; ++i)
   {
      struct rtVector3D b0 = {.x = faces->p[i].x - view->p.x,
                              .y = faces->p[i].y - view->p.y,
                              .z = faces->p[i].z - view->p.z};

      float rn = RT_DOT(view->r,faces->n[i]);

      // If rn value is very-very small, it means that the ray is gliding over
      // the face, and doesn't intersect it (so we skip this face)
      if (RT_ISZERO(rn)) { continue; }

      // Evaluate new value of t, and compare it with the previous minimal value, also
      // check that it's in the right direction
      tcur = RT_DOT(b0,faces->n[i]) / rn;

      if ((tcur > 0.0f) && tcur < tmin)
      {
         // If it's closer, than understand if the ray actually intersects the plane
         // within the face
         
         struct rtVector3D b1 = {.x = faces->e1[i].y * b0.z - faces->e1[i].z * b0.y,
                                 .y = faces->e1[i].z * b0.x - faces->e1[i].x * b0.z,
                                 .z = faces->e1[i].x * b0.y - faces->e1[i].y * b0.x};

         struct rtVector3D b2 = {.x = faces->e2[i].y * b0.z - faces->e1[i].z * b0.y,
                                 .y = faces->e2[i].z * b0.x - faces->e1[i].x * b0.z,
                                 .z = faces->e2[i].x * b0.y - faces->e1[i].y * b0.x};

         float ucur = RT_DOT(view->r,b2) / rn;
         float vcur = RT_DOT(view->r,b1) / rn;

         if (RT_INRANGE(0.0f,ucur,1.0f) && RT_INRANGE(0.0f,vcur,1.0f) && RT_INRANGE(0.0f,ucur+vcur,1.0f))
         {
            // Ray intersects the face; save new values
            umin = ucur; vmin = vcur;
            tmin = tcur;
            imin = i;
         }
      }
   }

   // Return the point of intersection through the pointer given in args
   intersection->x = view->p.x + view->r.x*tmin;
   intersection->y = view->p.y + view->r.y*tmin;
   intersection->z = view->p.z + view->r.z*tmin;
   
   return imin;
#endif
}

/* Similar function to the one above, but it finds if there's an obstacle between
   intersection point and the light source; if there is, it will make a shadow,
   and the light source won't contribute to the final color. Returns 0 if the point
   isn't exposed by the light of the light source, returns 1 otherwise. Also in 
   cosine returns cosine of the angle between the normal and the line connecting
   intersection and lightpoint (needed in the color evaluation). */
int rtIsExposed(struct rtFaces *faces,
                struct rtView *view,
                struct rtVector3D *lightpoint,
                int iface, struct rtVector3D *intersection,
                float* cosine)
{
#ifndef RT_SIMD
   // Form a vector of direction to the light source
   struct rtVector3D ray = {.x = lightpoint->x - intersection->x,
                            .y = lightpoint->y - intersection->y,
                            .z = lightpoint->z - intersection->z};
   // Note: it also has a length of the distance between the light source and the point
   // Thus we are only interested in finding intersection of ray with faces when t in (0;1)

   // Also, we need to check if the light source and the point of viewing
   // are on the same side from the face itself
   if (RT_DOT(ray,faces->n[iface]) * RT_DOT(view->r,faces->n[iface]) > 0)
   {
      // They are on different sides, so the point isn't exposed by the light source provided
      return 0;
   }

   for (int i = 0; i < faces->N; ++i)
   {
      struct rtVector3D b0 = {.x = faces->p[i].x - intersection->x,
                              .y = faces->p[i].y - intersection->y,
                              .z = faces->p[i].z - intersection->z};
      
      // If rn is close to zero, it means that light beams emitted from source are
      // gliding over the surface of the face, so this face isn't an obstacle.
      float rn = RT_DOT(ray,faces->n[i]);
      if (RT_ISZERO(rn)) { continue; }

      // The face itself can't be an obstacle to the light sources
      float b0n = RT_DOT(b0,faces->n[i]);
      if (RT_ISZERO(b0n)) { continue; }

      float t = b0n / rn;

      // Obstacle is something between the face (that's currently being checked for being exposed)
      // and the light source
      if (RT_INRANGE(0.0f,t,1.0f))
      {
         struct rtVector3D b1 = {.x = faces->e1[i].y * b0.z - faces->e1[i].z * b0.y,
                                 .y = faces->e1[i].z * b0.x - faces->e1[i].x * b0.z,
                                 .z = faces->e1[i].x * b0.y - faces->e1[i].y * b0.x};

         struct rtVector3D b2 = {.x = faces->e2[i].y * b0.z - faces->e1[i].z * b0.y,
                                 .y = faces->e2[i].z * b0.x - faces->e1[i].x * b0.z,
                                 .z = faces->e2[i].x * b0.y - faces->e1[i].y * b0.x};

         // Now we can evaluate the baricentric coordinates of the intersection with the plane of current face
         float u = RT_DOT(ray,b2) / rn;
         float v = RT_DOT(ray,b1) / rn;

         if (RT_INRANGE(0.0f,u,1.0f) && RT_INRANGE(0.0f,v,1.0f) && RT_INRANGE(0.0f,u+v,1.0f))
         {
            // ray intersects this face, so this face is an obstacle for the light beams exposing the
            // original intersection point (provided in args).
            return 0;
         }
      }
   }

   // If we haven't intersected any faces within a given range of t's, let's also evaluate the cosine
   // needed for calculation of the color
   float invnorm = 1.0f / sqrtf(RT_SQR(ray.x) + RT_SQR(ray.y) + RT_SQR(ray.z));
   struct rtVector3D uray = {.x = ray.x * invnorm,
                             .y = ray.y * invnorm,
                             .z = ray.z * invnorm};

   *cosine = fabsf(RT_DOT(uray,faces->un[iface]));
   return 1;
#endif
}