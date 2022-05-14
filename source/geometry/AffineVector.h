#ifndef __GEOMETRY_AFFINEVECTOR
#define __GEOMETRY_AFFINEVECTOR

#include "source/geometry/Vector.h"
#include "source/geometry/Point.h"

namespace raytracer
{
    struct AffineVector : public raytracer::Vector
    {
    // 3D Vector based on a point abstaction class
        raytracer::Point point;
        AffineVector(const raytracer::Point& point, const raytracer::Vector& vec) noexcept;
        AffineVector(const raytracer::Vector& vec) noexcept;
        AffineVector(const raytracer::Point& point) noexcept;
        AffineVector(const raytracer::Point& from, const raytracer::Point& to) noexcept;
    };
}

#endif // __GEOMETRY_AFFINEVECTOR