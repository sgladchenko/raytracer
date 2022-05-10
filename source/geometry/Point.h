#ifndef __GEOMETRY_POINT
#define __GEOMETRY_POINT

namespace raytracer
{
    class Point : public raytracer::PointData
    {
    // Point given by its cartesian coordinates
        using base = raytracer::PointData;
    public:
        Point() : base{} {}
        Point(double _x, double _y, double _z) : base(_x,_y,_z) {}

        double distance(const raytracer::Point& point) const;
        static double distance(const raytracer::Point& p1, const raytracer::Point& p2);
    };

    class PointData
    {
    // Data object storing Point data
    public:
        PointData() { coordinates[0] = 0; coordinates[1] = 0; coordinates[2] = 0; }
        PointData(double _x, double _y, double _z)
        {
            coordinates[0] = _x;
            coordinates[1] = _y;
            coordinates[2] = _z;
        }

        // Get methods
        double get_x() const { return coordinates[0]; }
        double get_y() const { return coordinates[1]; }
        double get_z() const { return coordinates[2]; }
        // Set methods
        void set_x(double val) { coordinates[0] = val; }
        void set_y(double val) { coordinates[1] = val; }
        void set_z(double val) { coordinates[2] = val; }
        // Get the actual pointer to the array
        double* get_array() { return coordinates; }

    private:
        double coordinates[3];
    };
}

#endif // __GEOMETRY_POINT