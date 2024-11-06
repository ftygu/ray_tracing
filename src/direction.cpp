#include "direction.hpp"
#include "point.hpp"
#include "vector3d.hpp"

Direction::Direction() : vector(Vector3d()) {}

Direction::Direction(double x, double y, double z) : vector(Vector3d(x, y, z)) {}

Direction::Direction(const Vector3d &v) : vector(v) {}

double Direction::x() const
{
    return vector[0];
}

double Direction::y() const
{
    return vector[1];
}

double Direction::z() const
{
    return vector[2];
}

Direction Direction::unit() const
{
    Vector3d unit_vector = vector.unit();
    return Direction(unit_vector);
}

float Direction::length() const
{
    return vector.length();
}

float Direction::length_squared() const
{
    return vector.length_squared();
}

float Direction::dot(const Direction &d) const
{
    return vector.dot(d.vector);
}

Direction Direction::cross(const Direction &d) const
{
    Vector3d cross_vector = vector.cross(d.vector);
    return Direction(cross_vector);
}

Direction Direction::operator+(const Direction &d) const
{
    return Direction(vector + d.vector);
}

Direction Direction::operator-(const Direction &d) const
{
    return Direction(vector - d.vector);
}

Pointer Direction::operator+(const Pointer &p) const
{
    return Pointer(p.x() + vector[0], p.y() + vector[1], p.z() + vector[2]);
}

Pointer Direction::operator-(const Pointer &p) const
{
    return Pointer(p.x() - vector[0], p.y() - vector[1], p.z() - vector[2]);
}

Direction Direction::operator*(double t) const
{
    return Direction(vector * t);
}

Direction Direction::operator/(double t) const
{
    return Direction(vector / t);
}
