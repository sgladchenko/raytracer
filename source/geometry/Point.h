#ifndef __GEOMETRY_POINT
#define __GEOMETRY_POINT

namespace raytracer
{
    // Basic object of a point given by its cartesian coordinates
    class Point
    {
    public:
        Point();
        Point(double _x, double _y, double _z);
        double distance(const raytracer::Point& point) const;
        static double distance(const raytracer::Point& p1, const raytracer::Point& p2);
    private:
        double x = 0;
        double y = 0;
        double z = 0;
    };
}

#endif // __GEOMETRY_POINT