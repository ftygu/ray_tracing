#pragma once

#include "direction.hpp"
#include "point.hpp"

class Ray
{
private:

    Point origin;
    Direction direction;

public:

    Ray();
    Ray(const Point &origin, const Direction &direction);

    Point get_origin() const;
    Direction get_direction() const;

    Point at(double t) const;
};