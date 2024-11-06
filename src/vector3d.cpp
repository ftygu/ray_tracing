#include "vector3d.hpp"

#include <cmath>

Vector3d::Vector3d() : e{0, 0, 0} {}

Vector3d::Vector3d(double e0, double e1, double e2) : e{e0, e1, e2} {}

double Vector3d::x() const { return e[0]; }

double Vector3d::y() const { return e[1]; }

double Vector3d::z() const { return e[2]; }

double Vector3d::length_squared() const
{
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
}

double Vector3d::length() const
{
    return std::sqrt(this->length_squared());
}

Vector3d Vector3d::unit() const
{
    double len = this->length();

    if (len == 0)
    {
        std::cerr << "ERROR: division by zero in Vector3d::unit()" << std::endl;
        exit(1);
    }

    return *this / len;

}

double Vector3d::operator[](int i) const { return e[i]; }

double& Vector3d::operator[](int i) { return e[i]; }

Vector3d Vector3d::operator-() const { return Vector3d(-e[0], -e[1], -e[2]); }

Vector3d Vector3d::operator+() const { return *this; }

Vector3d& Vector3d::operator+=(const Vector3d &v)
{
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];

    return *this;
}

Vector3d& Vector3d::operator-=(const Vector3d &v)
{
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];

    return *this;
}

Vector3d& Vector3d::operator*=(const Vector3d &v)
{
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];

    return *this;
}

Vector3d& Vector3d::operator/=(const Vector3d &v)
{
    if (v.e[0] == 0 || v.e[1] == 0 || v.e[2] == 0)
    {
        std::cerr << "ERROR: division by zero in Vector3d::operator/=(const Vector3d &v)" << std::endl;
        exit(1);
    }

    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];

    return *this;
}

Vector3d& Vector3d::operator+=(double t)
{
    e[0] += t;
    e[1] += t;
    e[2] += t;

    return *this;
}

Vector3d& Vector3d::operator-=(double t)
{
    e[0] -= t;
    e[1] -= t;
    e[2] -= t;

    return *this;
}

Vector3d& Vector3d::operator*=(double t)
{
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;

    return *this;
}

Vector3d& Vector3d::operator/=(double t)
{

    if (t == 0)
    {
        std::cerr << "ERROR: division by zero in Vector3d::operator/=(double t)" << std::endl;
        exit(1);
    }

    e[0] /= t;
    e[1] /= t;
    e[2] /= t;

    return *this;
}

Vector3d Vector3d::operator+(const Vector3d &v) const
{
    return Vector3d(e[0] + v.e[0], e[1] + v.e[1], e[2] + v.e[2]);
}

Vector3d Vector3d::operator-(const Vector3d &v) const
{
    return Vector3d(e[0] - v.e[0], e[1] - v.e[1], e[2] - v.e[2]);
}

Vector3d Vector3d::operator*(const Vector3d &v) const
{
    return Vector3d(e[0] * v.e[0], e[1] * v.e[1], e[2] * v.e[2]);
}

Vector3d Vector3d::operator/(const Vector3d &v) const
{

    if (v.e[0] == 0 || v.e[1] == 0 || v.e[2] == 0)
    {
        std::cerr << "ERROR: division by zero in Vector3d::operator/(const Vector3d &v)" << std::endl;
        exit(1);
    }

    return Vector3d(e[0] / v.e[0], e[1] / v.e[1], e[2] / v.e[2]);
}

Vector3d Vector3d::operator+(double t) const
{
    return Vector3d(e[0] + t, e[1] + t, e[2] + t);
}

Vector3d Vector3d::operator-(double t) const
{
    return Vector3d(e[0] - t, e[1] - t, e[2] - t);
}

Vector3d Vector3d::operator*(double t) const
{
    return Vector3d(e[0] * t, e[1] * t, e[2] * t);
}

Vector3d Vector3d::operator/(double t) const
{

    if (t == 0)
    {
        std::cerr << "ERROR: division by zero in Vector3d::operator/(double t)" << std::endl;
        exit(1);
    }

    return Vector3d(e[0] / t, e[1] / t, e[2] / t);
}

double Vector3d::dot(const Vector3d &v) const
{
    return e[0] * v.e[0] + e[1] * v.e[1] + e[2] * v.e[2];
}

Vector3d Vector3d::cross(const Vector3d &v) const
{
    return Vector3d(e[1] * v.e[2] - e[2] * v.e[1],
                    e[2] * v.e[0] - e[0] * v.e[2],
                    e[0] * v.e[1] - e[1] * v.e[0]);
}