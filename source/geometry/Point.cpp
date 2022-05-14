#include "source/geometry/Point.h"
#include "source/Macro.h"

#include <cmath>

raytracer::Point::Point(double _x, double _y, double _z) noexcept
: coordinates {_x,_y,_z}
{}

inline double raytracer::Point::get_x()
const noexcept
{ return coordinates[0]; }

inline double raytracer::Point::get_y()
const noexcept
{ return coordinates[1]; }

inline double raytracer::Point::get_z()
const noexcept
{ return coordinates[2]; }

inline void raytracer::Point::set_x(double val)
noexcept
{ coordinates[0] = val; }

inline void raytracer::Point::set_y(double val)
noexcept
{ coordinates[1] = val; }

inline void raytracer::Point::set_z(double val)
noexcept
{ coordinates[2] = val; }

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
    return raytracer::Point::distance(*this, point);
}