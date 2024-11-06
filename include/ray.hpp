#pragma once

#include "direction.hpp"
#include "point.hpp"

class Ray
{
private:

    Pointer origin;
    Direction direction;

public:

    Ray();
    Ray(const Pointer &origin, const Direction &direction);

    Pointer get_origin() const;
    Direction get_direction() const;

    Pointer at(double t) const;
};