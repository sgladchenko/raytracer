#ifndef __GEOMETRY_SIMPLEXPOLYGON
#define __GEOMETRY_SIMPLEXPOLYGON

#include <memory>
#include "source/geometry/BasePolygon.h"
#include "source/geometry/Ray.h"
#include "source/geometry/Point.h"
#include "source/geometry/Vector.h"

namespace raytracer
{
    // Abstraction of a triangular polygon
    class SimplexPolygon : public raytracer::BasePolygon
    {
    public:
        SimplexPolygon() = delete;
        bool hits(const raytracer::Ray& ray) const override;
        bool hits(const raytracer::Ray& ray, std::unique_ptr<raytracer::Point> point) const override;
    private:
        // Planar simplex is defined by a base point and two vectors which it's spanned on
        raytracer::Point basePoint;
        raytracer::Point vector1;
        raytracer::Point vector2;
        // Cached values

    };
}

#endif // __GEOMETRY_SIMPLEXPOLYGON