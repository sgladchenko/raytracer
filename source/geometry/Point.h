#ifndef __GEOMETRY_POINT
#define __GEOMETRY_POINT

#include "source/Macro.h"

namespace raytracer
{
    class Point
    {
    // Point given by its cartesian coordinates
    public:
        Point(double _x, double _y, double _z) noexcept;

        // Get methods
        inline double get_x() const noexcept;
        inline double get_y() const noexcept;
        inline double get_z() const noexcept;

        // Set methods
        inline void set_x(double val) noexcept;
        inline void set_y(double val) noexcept;
        inline void set_z(double val) noexcept;

        double distance(const raytracer::Point& point) const noexcept;
        static double distance(const raytracer::Point& p1, const raytracer::Point& p2) noexcept;

        // Some useful constants 
        static const raytracer::Point origin;

    protected:
        RT_3DARRAY(double,coordinates);
    };
}

#endif // __GEOMETRY_POINT