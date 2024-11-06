#pragma once

#include "vector3d.hpp"

class Color
{
private:

    Vector3d vector;

public:
    Color();
    Color(double r, double g, double b);
    Color(const Vector3d &v);

    double r() const;

    double g() const;

    double b() const;

    void write_as_ppm(std::ostream &out) const;

    Color operator+(const Color &c) const;
    Color operator-(const Color &c) const;
    Color operator*(const Color &c) const;
    Color operator/(const Color &c) const;

    Color operator*(double t) const;
    Color operator/(double t) const;
    Color operator+(double t) const;
    Color operator-(double t) const;
};