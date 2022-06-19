#include "rtcore_internal.h"

#include "math.h"

/* Returns index of the face appeared to be closest one to the point of view in the direction
   of viewing (t>0); returns -1 if there's no face intersected. */
int rtIntersect(const rtFaces *faces, const rtView *view, rtVector3D* intersection)
{
#ifndef RT_SIMD
   // Provide some invalid values, just to avoid any uninitialized reads
   float tcur, tmin, ucur, umin, vcur, vmin;
   tcur = tmin = INFINITY;
   ucur = umin = vcur = vmin = -1.0f;
   int imin = -1; // Will be returned if no intersections found

   for (int i = 0; i < faces->N; ++i)
   {
      rtVector3D b0 = {.x = faces->p[i].x - view->p.x,
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
         
         rtVector3D b1 = {.x = faces->e1[i].y * b0.z - faces->e1[i].z * b0.y,
                          .y = faces->e1[i].z * b0.x - faces->e1[i].x * b0.z,
                          .z = faces->e1[i].x * b0.y - faces->e1[i].y * b0.x};

         rtVector3D b2 = {.x = faces->e2[i].y * b0.z - faces->e1[i].z * b0.y,
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
   and the light source won't contribute to the final color. */
int rtFindShadow(const rtFaces *faces, const rtVector3D *lightpoint, const rtVector3D *intersection)
{
#ifndef RT_SIMD
   
#endif
}