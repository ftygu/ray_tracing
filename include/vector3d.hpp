#pragma once

#include <cmath>
#include <iostream>
class Vector3d
{
private:

    double e[3];

public:

    Vector3d();

    Vector3d(double e0, double e1, double e2);
    
    double x() const;

    double y() const;

    double z() const;

    double length_squared() const;

    double length() const;

    double operator[](int i) const;
    double& operator[](int i);

    Vector3d operator-() const;
    Vector3d operator+() const;

    Vector3d& operator+=(const Vector3d &v);
    Vector3d& operator-=(const Vector3d &v);
    Vector3d& operator*=(const Vector3d &v);
    Vector3d& operator/=(const Vector3d &v);

    Vector3d& operator+=(double t);
    Vector3d& operator-=(double t);
    Vector3d& operator*=(double t);
    Vector3d& operator/=(double t);

    Vector3d operator+(const Vector3d &v) const;
    Vector3d operator-(const Vector3d &v) const;
    Vector3d operator*(const Vector3d &v) const;
    Vector3d operator/(const Vector3d &v) const;

    Vector3d operator+(double t) const;
    Vector3d operator-(double t) const;
    Vector3d operator*(double t) const;
    Vector3d operator/(double t) const;

    double dot(const Vector3d &v) const;
    Vector3d cross(const Vector3d &v) const;

    Vector3d unit() const;
};