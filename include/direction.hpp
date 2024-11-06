#pragma once

#include "vector3d.hpp"

class Pointer;

class Direction
{
private:

    Vector3d vector;

public:

    Direction();
    Direction(double x, double y, double z);
    Direction(const Vector3d &v);

    double x() const;
    double y() const;
    double z() const;

    Direction unit() const;

    float length() const;
    float length_squared() const;

    float dot(const Direction &d) const;
    Direction cross(const Direction &d) const;

    Direction operator+(const Direction &d) const;
    Direction operator-(const Direction &d) const;

    Pointer operator+(const Pointer &p) const;
    Pointer operator-(const Pointer &p) const;

    Direction operator*(double t) const;
    Direction operator/(double t) const;
};