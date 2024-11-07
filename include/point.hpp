#pragma once

#include "vector3d.hpp"

class Direction;

class Point
{
private:

    Vector3d vector;

public:
    Point();
    Point(double x, double y, double z);
    Point(const Vector3d &v);

    double x() const;

    double y() const;

    double z() const;

    Point operator+(const Direction &d) const;
    Point operator-(const Direction &d) const;

    Direction operator-(const Point &p) const;
};