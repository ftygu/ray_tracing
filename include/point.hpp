#pragma once

#include "vector3d.hpp"

class Direction;

class Pointer
{
private:

    Vector3d vector;

public:
    Pointer();
    Pointer(double x, double y, double z);
    Pointer(const Vector3d &v);

    double x() const;

    double y() const;

    double z() const;

    Pointer operator+(const Direction &d) const;
    Pointer operator-(const Direction &d) const;

    Direction operator-(const Pointer &p) const;
};