#ifndef __GEOMETRY_BASEPOLYGON
#define __GEOMETRY_BASEPOLYGON

#include <memory>
#include "source/geometry/Ray.h"
#include "source/geometry/Point.h"

namespace raytracer
{
    // Basic interface for all the polygons to be used in further
    class BasePolygon
    {
    public:
        virtual bool hits(const raytracer::Ray& ray) const = 0;
        virtual bool hits(const raytracer::Ray& ray, std::unique_ptr<raytracer::Point> point) const = 0;
    protected:
    private:
    };
}

#endif // __GEOMETRY_BASEPOLYGON