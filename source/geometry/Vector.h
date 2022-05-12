#ifndef __GEOMETRY_VECTOR
#define __GEOMETRY_VECTOR

#include "source/geometry/Point.h"
#include "source/Macro.h"
#include "source/Cached.h"

namespace raytracer
{
    class Vector
    {
    // 3D Vector given by its components in a cartesian frame
    public:
        Vector() noexcept;
        Vector(double _vx, double _vy, double _vz) noexcept;
        Vector(const raytracer::Point& from, const raytracer::Point& to) noexcept;
        #ifdef RT_UREADS
            Vector(const raytracer::UnitVector& _unitVector) noexcept;
            Vector(const raytracer::UnitVector& _unitVector, double _norm) noexcept;
        #else
            Vector(const raytracer::UnitVector& _unitVector);
            Vector(const raytracer::UnitVector& _unitVector, double _norm);
        #endif

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
        friend double raytracer::dot(const raytracer::Vector& v1, const raytracer::Vector& v2) noexcept;
        #ifdef RT_UREADS
            friend double raytracer::cos(const raytracer::Vector& v1, const raytracer::Vector& v2) noexcept;
        #else
            friend double raytracer::cos(const raytracer::Vector& v1, const raytracer::Vector& v2);
        #endif
    
    protected:
        RT_3DARRAY(double,coordinates);
        mutable raytracer::Cached<double> norm;
        mutable raytracer::Cached<raytracer::UnitVector> unitVector;
    
        void update_norm() const noexcept;
        void update_unitVector() const noexcept;
    };

    class UnitVector
    {
    // 3D Vector of the unit length, given by its components in a cartesian frame
    public:
        UnitVector() noexcept;
        UnitVector(double _vx, double _vy, double _vz) noexcept; // It will re-scale them to unit

        // Basic linear operations
        #ifdef RT_UREADS
            raytracer::Vector operator+(const raytracer::Vector& rhs) const noexcept;
            raytracer::Vector operator-(const raytracer::Vector& rhs) const noexcept;
            raytracer::Vector operator*(double rhs) const noexcept;
            raytracer::Vector operator/(double rhs) const noexcept;
        #else
            raytracer::Vector operator+(const raytracer::Vector& rhs) const;
            raytracer::Vector operator-(const raytracer::Vector& rhs) const;
            raytracer::Vector operator*(double rhs) const;
            raytracer::Vector operator/(double rhs) const;
        #endif

        // Some useful constants
        static const raytracer::UnitVector e_x;
        static const raytracer::UnitVector e_y;
        static const raytracer::UnitVector e_z;

        // Make it friend, so we'll be able to use vector intrinsics if needed
        #ifdef RT_UREADS
            friend double raytracer::dot(const raytracer::UnitVector& v1, const raytracer::UnitVector& v2) noexcept;
        #else
            friend double raytracer::dot(const raytracer::UnitVector& v1, const raytracer::UnitVector& v2);
        #endif

    protected:
        RT_3DARRAY(double,coordinates);
        #ifndef RT_UREADS
            bool isdefined;
        #endif
    };

    // Some other useful binary operations over vectors
    #ifdef RT_UREADS
        double dot(const raytracer::UnitVector& v1, const raytracer::UnitVector& v2) noexcept;
        double cos(const raytracer::Vector& v1, const raytracer::Vector& v2) noexcept;
    #else
        double dot(const raytracer::UnitVector& v1, const raytracer::UnitVector& v2);
        double cos(const raytracer::Vector& v1, const raytracer::Vector& v2);
    #endif
    double dot(const raytracer::Vector& v1, const raytracer::Vector& v2) noexcept;
}

#endif // __GEOMETRY_VECTOR