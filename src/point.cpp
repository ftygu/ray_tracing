#include "point.hpp"
#include "direction.hpp"

Pointer::Pointer() : vector(Vector3d()) {}

Pointer::Pointer(double x, double y, double z) : vector(Vector3d(x, y, z)) {}

Pointer::Pointer(const Vector3d &v) : vector(v) {}

double Pointer::x() const
{
    return vector[0];
}

double Pointer::y() const
{
    return vector[1];
}

double Pointer::z() const
{
    return vector[2];
}

Pointer Pointer::operator+(const Direction &d) const
{
    return Pointer(vector[0] + d.x(), vector[1] + d.y(), vector[2] + d.z());
}

Pointer Pointer::operator-(const Direction &d) const
{
    return Pointer(vector[0] - d.x(), vector[1] - d.y(), vector[2] - d.z());
}

Direction Pointer::operator-(const Pointer &p) const
{
    return Direction(vector - p.vector);
}