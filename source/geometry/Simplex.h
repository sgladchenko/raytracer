#ifndef __GEOMETRY_SIMPLEX
#define __GEOMETRY_SIMPLEX

#include <memory>
#include "source/geometry/BasePolygon.h"
#include "source/geometry/Ray.h"
#include "source/geometry/Point.h"

namespace raytracer
{
    // Abstraction of a triangular polygon
    class Simplex : public raytracer::BasePolygon
    {
    public:
        Simplex() = delete;
        bool hits(const raytracer::Ray& ray) const override;
        bool hits(const raytracer::Ray& ray, std::unique_ptr<raytracer::Point> point) const override;
    private:
    };
}

#endif // __GEOMETRY_SIMPLEX