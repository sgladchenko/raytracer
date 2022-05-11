#ifndef __GEOMETRY_AFFINEVECTOR
#define __GEOMETRY_AFFINEVECTOR

#include "source/geometry/Vector.h"
#include "source/geometry/Point.h"

namespace raytracer
{
    class AffineVector : public raytracer::Vector
    {
    public:
        AffineVector();
        AffineVector(const raytracer::Point& point, const raytracer::Vector& vec);
        AffineVector(const raytracer::Vector& vec);
        AffineVector(const raytracer::Point& point);
        AffineVector(const raytracer::Point& from, const raytracer::Point& to);
    
    private:
        raytracer::Point basePoint;
    };
}

#endif // __GEOMETRY_AFFINEVECTOR