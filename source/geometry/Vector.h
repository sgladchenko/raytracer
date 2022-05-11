#ifndef __GEOMETRY_VECTOR
#define __GEOMETRY_VECTOR

#include "source/geometry/Point.h"
#include "source/Cached.h"

namespace raytracer
{
    class Vector
    {
    // 3D Vector given by its components in a cartesian frame
    public:
        Vector(); // no inits of doubles and cache
        Vector(double _vx, double _vy, double _vz);
        Vector(const raytracer::UnitVector& _unitVector);
        Vector(const raytracer::UnitVector& _unitVector, double _norm);
        Vector(const raytracer::Point& from, const raytracer::Point& to);

        // Get methods
        double get_x() const noexcept;
        double get_y() const noexcept;
        double get_z() const noexcept;

        // Set methods
        void set_x(double val) noexcept;
        void set_y(double val) noexcept;
        void set_z(double val) noexcept;

        // Dependent properties
        double get_norm() const noexcept;
        const raytracer::UnitVector& get_unitVector() const;

        // Basic linear operations
        raytracer::Vector operator+(const raytracer::Vector& rhs) const noexcept;
        raytracer::Vector operator-(const raytracer::Vector& rhs) const noexcept;
        raytracer::Vector operator*(double rhs) const noexcept;
        raytracer::Vector operator/(double rhs) const noexcept;

        // More complicated linear operations

        // Some useful constants
        static const raytracer::Vector zero;
        static const raytracer::Vector e_x;
        static const raytracer::Vector e_y;
        static const raytracer::Vector e_z;
    
    protected:
        RT_3DARRAY(double,coordinates);
        mutable raytracer::Cached<double> norm;
        mutable raytracer::Cached<raytracer::UnitVector> unitVector;
    
        void update_norm() const noexcept;
        void update_unitVector() const;
    };

    class UnitVector
    {
    // 3D Vector of the unit length, given by its components in a cartesian frame
    public:
        UnitVector(); // no inits of doubles
        UnitVector(double _vx, double _vy, double _vz); // It will re-scale them to unit

        // Basic linear operations
        raytracer::Vector operator+(const raytracer::Vector& rhs) const noexcept;
        raytracer::Vector operator-(const raytracer::Vector& rhs) const noexcept;
        raytracer::Vector operator*(double rhs) const noexcept;
        raytracer::Vector operator/(double rhs) const noexcept;

        // Some useful constants
        static const raytracer::UnitVector e_x;
        static const raytracer::UnitVector e_y;
        static const raytracer::UnitVector e_z;

    protected:
        RT_3DARRAY(double,coordinates);
    };
}

#endif // __GEOMETRY_VECTOR