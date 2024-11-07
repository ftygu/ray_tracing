#include "ray.hpp"

Ray::Ray() : origin(Point()), direction(Direction()) {}

Ray::Ray(const Point &origin, const Direction &direction) : origin(origin), direction(direction) {}

Point Ray::get_origin() const
{
    return origin;
}

Direction Ray::get_direction() const
{
    return direction;
}

Point Ray::at(double t) const
{
    return origin + direction * t;
}