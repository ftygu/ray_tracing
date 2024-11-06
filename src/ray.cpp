#include "ray.hpp"

Ray::Ray() : origin(Pointer()), direction(Direction()) {}

Ray::Ray(const Pointer &origin, const Direction &direction) : origin(origin), direction(direction) {}

Pointer Ray::get_origin() const
{
    return origin;
}

Direction Ray::get_direction() const
{
    return direction;
}

Pointer Ray::at(double t) const
{
    return origin + direction * t;
}