#include "source/geometry/Vector.h"
#include "source/Macro.h"

#include <cmath>

raytracer::Vector::Vector(double _vx, double _vy, double _vz) noexcept
: components {_vx,_vy,_vz}
{}

raytracer::Vector::Vector(const raytracer::Point& from, const raytracer::Point& to) noexcept
: components {to.get_x()-from.get_x(),
              to.get_y()-from.get_y(),
              to.get_z()-from.get_z()}
{}

raytracer::Vector::Vector(const raytracer::UnitVector& _unitVector) noexcept
{

}
