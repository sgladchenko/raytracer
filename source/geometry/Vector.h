#ifndef __GEOMETRY_VECTOR
#define __GEOMETRY_VECTOR

#include "source/geometry/Point.h"
#include "source/Macro.h"
#include "source/Cached.h"

namespace raytracer
{
    class Vector;
    class UnitVector;
}

namespace raytracer
{
    double cos(const raytracer::Vector& v1, const raytracer::Vector& v2);
    double dot(const raytracer::UnitVector& v1, const raytracer::UnitVector& v2) noexcept;
    double dot(const raytracer::Vector& v1, const raytracer::Vector& v2) noexcept;
}

namespace raytracer
{
    class Vector
    {
    // 3D Vector given by its components in a cartesian frame
    public:
        Vector(double _vx, double _vy, double _vz) noexcept;
        Vector(const raytracer::Point& from, const raytracer::Point& to) noexcept;
        Vector(const raytracer::UnitVector& _unitVector) noexcept;
        Vector(const raytracer::UnitVector& _unitVector, double _norm) noexcept;

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
        const raytracer::UnitVector& get_unitVector() const noexcept;

        // Basic linear operations
        raytracer::Vector operator+(const raytracer::Vector& rhs) const noexcept;
        raytracer::Vector operator-(const raytracer::Vector& rhs) const noexcept;
        raytracer::Vector operator*(double rhs) const noexcept;
        raytracer::Vector operator/(double rhs) const;

        // Some useful constants
        static const raytracer::Vector zero;
        static const raytracer::Vector e_x;
        static const raytracer::Vector e_y;
        static const raytracer::Vector e_z;
    
        // Make it friend, so we'll be able to use vector intrinsics if needed
        friend double raytracer::cos(const raytracer::Vector& v1, const raytracer::Vector& v2);
        friend double raytracer::dot(const raytracer::Vector& v1, const raytracer::Vector& v2) noexcept;

        // Exception to be raised whenever we try to operate over a UnitVector
        // which cannot be created out of a normal vector
        struct UndefinedUnitVectorException : public std::exception {};
        
    protected:
        RT_3DARRAY(double,components);
        mutable raytracer::Cached<double> norm;
        mutable raytracer::Cached<raytracer::UnitVector> unitVector;
            
        void update_norm() const noexcept;
        void update_unitVector() const noexcept;
    };
}

namespace raytracer
{
    class UnitVector
    {
    // 3D Vector of the unit length, given by its components in a cartesian frame
    public:
        UnitVector(double _vx, double _vy, double _vz) noexcept; // it will re-scale them to unit
        
        // Get methods
        double get_x() const noexcept;
        double get_y() const noexcept;
        double get_z() const noexcept;

        // Set methods
        void set_x(double val) noexcept;
        void set_y(double val) noexcept;
        void set_z(double val) noexcept;

        // Basic linear operations
        raytracer::Vector operator+(const raytracer::Vector& rhs) const noexcept;
        raytracer::Vector operator-(const raytracer::Vector& rhs) const noexcept;
        raytracer::Vector operator*(double rhs) const noexcept;
        raytracer::Vector operator/(double rhs) const noexcept;

        // Some useful constants
        static const raytracer::UnitVector e_x;
        static const raytracer::UnitVector e_y;
        static const raytracer::UnitVector e_z;

        // Make it friend, so we'll be able to use vector intrinsics if needed
        friend double raytracer::dot(const raytracer::UnitVector& v1, const raytracer::UnitVector& v2) noexcept;

    protected:
        RT_3DARRAY(double,coordinates);
    };

    // Some other useful binary operations over vectors
}

#endif // __GEOMETRY_VECTOR