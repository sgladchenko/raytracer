#include "source/geometry/Vector.h"
#include "source/Macro.h"

#include <cmath>
#include <cstring>

//////////////////////////////////
// Vector interface definition  //
//////////////////////////////////

const raytracer::Vector raytracer::Vector::zero {0.0, 0.0, 0.0};
const raytracer::Vector raytracer::Vector::e_x  {1.0, 0.0, 0.0};
const raytracer::Vector raytracer::Vector::e_y  {0.0, 1.0, 0.0};
const raytracer::Vector raytracer::Vector::e_z  {0.0, 0.0, 1.0};

raytracer::Vector::Vector(double _vx, double _vy, double _vz) noexcept
: components {_vx,_vy,_vz}
{}

raytracer::Vector::Vector(double* arr) noexcept
{
    std::memcpy(components, arr, sizeof(double)*RT_3DARRAY_SIZE);
}

raytracer::Vector::Vector(const raytracer::Point& from, const raytracer::Point& to) noexcept
: components {to.get_x()-from.get_x(),
              to.get_y()-from.get_y(),
              to.get_z()-from.get_z()}
{}

raytracer::Vector::Vector(const raytracer::UnitVector& _unitVector) noexcept
{
    std::memcpy(components, _unitVector.components, sizeof(double)*RT_3DARRAY_SIZE);
    norm.actualize(1.0);
    unitVector.actualize(_unitVector);
}

raytracer::Vector::Vector(const raytracer::UnitVector& _unitVector, double _norm) noexcept
: components {_unitVector.get_x()*_norm,
              _unitVector.get_y()*_norm,
              _unitVector.get_z()*_norm }
{
    norm.actualize(_norm);
    unitVector.actualize(_unitVector);
}

void raytracer::Vector::set_x(double val) noexcept
{
    components[0] = val;
    norm.expire(); unitVector.expire();
}

void raytracer::Vector::set_y(double val) noexcept
{
    components[1] = val;
    norm.expire(); unitVector.expire();
}

void raytracer::Vector::set_z(double val) noexcept
{
    components[2] = val;
    norm.expire(); unitVector.expire();
}

void raytracer::Vector::set_array(double* arr) noexcept
{
    std::memcpy(components, arr, sizeof(double)*RT_3DARRAY_SIZE);
    norm.expire(); unitVector.expire();
}

double raytracer::Vector::get_norm() const noexcept
{
    if (!norm.actual) { update_norm(); }
    return norm.value;
}

const raytracer::UnitVector& raytracer::Vector::get_unitVector() const
{
    if (!unitVector.actual) { update_unitVector(); }
    return unitVector.value;
}

raytracer::Vector
raytracer::Vector::operator+(const raytracer::Vector& rhs) const noexcept
{
    raytracer::Vector res;
    for (std::size_t i=0; i<RT_3DARRAY_SIZE; ++i)
    { res.components[i] = components[i] + rhs.components[i]; }
    return res;
}

raytracer::Vector
raytracer::Vector::operator-(const raytracer::Vector& rhs) const noexcept
{
    raytracer::Vector res;
    for (std::size_t i=0; i<RT_3DARRAY_SIZE; ++i)
    { res.components[i] = components[i] - rhs.components[i]; }
    return res;
}

raytracer::Vector
raytracer::Vector::operator*(double rhs) const noexcept
{
    raytracer::Vector res;
    for (std::size_t i=0; i<RT_3DARRAY_SIZE; ++i)
    { res.components[i] = components[i] * rhs; }
    return res;
}

raytracer::Vector
raytracer::Vector::operator/(double rhs) const
{
    raytracer::Vector res;
    for (std::size_t i=0; i<RT_3DARRAY_SIZE; ++i)
    { res.components[i] = components[i] / rhs; }
    return res;
}

//////////////////////////////////////
// UnitVector interface definition  //
//////////////////////////////////////

const raytracer::UnitVector raytracer::UnitVector::e_x {1.0, 0.0, 0.0};
const raytracer::UnitVector raytracer::UnitVector::e_y {0.0, 1.0, 0.0};
const raytracer::UnitVector raytracer::UnitVector::e_z {0.0, 0.0, 1.0};

raytracer::UnitVector::UnitVector(double _vx, double _vy, double _vz) noexcept
{}