#ifndef __GEOMETRY_VECTOR
#define __GEOMETRY_VECTOR

#include "source/geometry/Point.h"
#include "source/Cached.h"

namespace raytracer
{
    class Vector : public raytracer::VectorData
    {
    // 3D Vector given by its components in a cartesian frame
        using base = raytracer::VectorData;
    public:
        Vector() : base{} {}
        Vector(double _vx, double _vy, double _vz);
        Vector(const raytracer::Point& from, const raytracer::Point& to);

        virtual raytracer::Vector operator+(const raytracer::Vector& rhs) const;
        virtual raytracer::Vector operator-(const raytracer::Vector& rhs) const;
        virtual raytracer::Vector operator*(double rhs) const;
        virtual raytracer::Vector operator/(double rhs) const;
    };

    class VectorData
    {
    // Data object storing Vector data
    public:
        VectorData() { components[0] = 0; components[1] = 0; components[2] = 0; }
        VectorData(double _vx, double _vy, double _vz)
        {
            components[0] = _vx;
            components[1] = _vy;
            components[2] = _vz;
        }

        // Get methods
        double get_x() const { return components[0]; }
        double get_y() const { return components[1]; }
        double get_z() const { return components[2]; }
        // Set methods
        void set_x(double val) { components[0] = val; }
        void set_y(double val) { components[1] = val; }
        void set_z(double val) { components[2] = val; }
        // Get the actual pointer to the array
        double* get_array() { return components; }

    private:
        double components[3];
        raytracer::Cached<double> norm;
    };
}

#endif // __GEOMETRY_VECTOR