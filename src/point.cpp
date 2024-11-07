#include "point.hpp"
#include "direction.hpp"

Point::Point() : vector(Vector3d()) {}

Point::Point(double x, double y, double z) : vector(Vector3d(x, y, z)) {}

Point::Point(const Vector3d &v) : vector(v) {}

double Point::x() const
{
    return vector[0];
}

double Point::y() const
{
    return vector[1];
}

double Point::z() const
{
    return vector[2];
}

Point Point::operator+(const Direction &d) const
{
    return Point(vector[0] + d.x(), vector[1] + d.y(), vector[2] + d.z());
}

Point Point::operator-(const Direction &d) const
{
    return Point(vector[0] - d.x(), vector[1] - d.y(), vector[2] - d.z());
}

Direction Point::operator-(const Point &p) const
{
    return Direction(vector - p.vector);
}