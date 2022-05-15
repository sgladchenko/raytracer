#include "source/geometry/Point.h"
#include "source/Macro.h"

#include <cmath>

const raytracer::Point raytracer::Point::origin {0.0, 0.0, 0.0};

raytracer::Point::Point(double _x, double _y, double _z) noexcept
: coordinates {_x,_y,_z}
{}

double raytracer::Point::distance(const raytracer::Point& p1, const raytracer::Point& p2)
noexcept
{
    return std::sqrt(RT_SQR(p1.get_x() - p2.get_x()) + 
                     RT_SQR(p1.get_y() - p2.get_y()) + 
                     RT_SQR(p1.get_z() - p2.get_z()));
}

double raytracer::Point::distance(const raytracer::Point& point)
const noexcept
{
    return distance(*this, point);
}