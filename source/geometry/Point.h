#ifndef __GEOMETRY_POINT
#define __GEOMETRY_POINT

#include "source/Macro.h"

namespace raytracer
{
    class Point
    {
    // Point given by its cartesian coordinates
    public:
        Point() {}
        Point(double _x, double _y, double _z) noexcept;

        // Get methods
        double get_x() const noexcept { return coordinates[0]; }
        double get_y() const noexcept { return coordinates[1]; }
        double get_z() const noexcept { return coordinates[2]; }

        // Set methods
        void set_x(double val) noexcept { coordinates[0] = val; }
        void set_y(double val) noexcept { coordinates[1] = val; }
        void set_z(double val) noexcept { coordinates[2] = val; }

        double distance(const raytracer::Point& point) const noexcept;
        static double distance(const raytracer::Point& p1, const raytracer::Point& p2) noexcept;

        // Some useful constants 
        static const raytracer::Point origin;

    protected:
        RT_3DARRAY(double,coordinates);
    };
}

#endif // __GEOMETRY_POINT