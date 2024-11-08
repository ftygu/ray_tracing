#pragma once

#include <cmath>
#include <iostream>
class Vector3d
{
private:

    double e[3];

public:

    Vector3d() : e{0, 0, 0} {}

    Vector3d(double e0, double e1, double e2) : e{e0, e1, e2} {}
    
    inline double x() const
    {
        return e[0];
    }

    inline double y() const
    {
        return e[1];
    }

    inline double z() const
    {
        return e[2];
    }

    inline double length_squared() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }   

    inline double length() const
    {
        return std::sqrt(this->length_squared());
    }

    inline double operator[](int i) const
    {
        return e[i];
    }

    inline double& operator[](int i)
    {
        return e[i];
    }

    inline Vector3d operator-() const
    {
        return Vector3d(-e[0], -e[1], -e[2]);
    }

    inline Vector3d operator+() const
    {
        return *this;
    }

    inline Vector3d& operator+=(const Vector3d &v)
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];

        return *this;
    }

    inline Vector3d& operator-=(const Vector3d &v)
    {
        e[0] -= v.e[0];
        e[1] -= v.e[1];
        e[2] -= v.e[2];

        return *this;
    }

    inline Vector3d& operator*=(const Vector3d &v)
    {
        e[0] *= v.e[0];
        e[1] *= v.e[1];
        e[2] *= v.e[2];

        return *this;
    }

    inline Vector3d& operator/=(const Vector3d &v)
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

    inline Vector3d& operator+=(double t)
    {
        e[0] += t;
        e[1] += t;
        e[2] += t;

        return *this;
    }

    inline Vector3d& operator-=(double t)
    {
        e[0] -= t;
        e[1] -= t;
        e[2] -= t;

        return *this;
    }

    inline Vector3d& operator*=(double t)
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;

        return *this;
    }

    inline Vector3d& operator/=(double t)
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

    inline Vector3d operator+(const Vector3d &v) const
    {
        return Vector3d(e[0] + v.e[0], e[1] + v.e[1], e[2] + v.e[2]);
    }

    inline Vector3d operator-(const Vector3d &v) const
    {
        return Vector3d(e[0] - v.e[0], e[1] - v.e[1], e[2] - v.e[2]);
    }

    inline Vector3d operator*(const Vector3d &v) const
    {
        return Vector3d(e[0] * v.e[0], e[1] * v.e[1], e[2] * v.e[2]);
    }

    inline Vector3d operator/(const Vector3d &v) const
    {
        if (v.e[0] == 0 || v.e[1] == 0 || v.e[2] == 0)
        {
            std::cerr << "ERROR: division by zero in Vector3d::operator/(const Vector3d &v)" << std::endl;
            exit(1);
        }

        return Vector3d(e[0] / v.e[0], e[1] / v.e[1], e[2] / v.e[2]);
    }

    inline Vector3d operator+(double t) const
    {
        return Vector3d(e[0] + t, e[1] + t, e[2] + t);
    }

    inline Vector3d operator-(double t) const
    {
        return Vector3d(e[0] - t, e[1] - t, e[2] - t);
    }

    inline Vector3d operator*(double t) const
    {
        return Vector3d(e[0] * t, e[1] * t, e[2] * t);
    }

    inline Vector3d operator/(double t) const
    {
        if (t == 0)
        {
            std::cerr << "ERROR: division by zero in Vector3d::operator/(double t)" << std::endl;
            exit(1);
        }

        return Vector3d(e[0] / t, e[1] / t, e[2] / t);
    }

    inline double dot(const Vector3d &v) const
    {
        return e[0] * v.e[0] + e[1] * v.e[1] + e[2] * v.e[2];
    }

    inline Vector3d cross(const Vector3d &v) const
    {
        return Vector3d(e[1] * v.e[2] - e[2] * v.e[1],
                        e[2] * v.e[0] - e[0] * v.e[2],
                        e[0] * v.e[1] - e[1] * v.e[0]);
    }

    inline Vector3d unit() const
    {
        return *this / this->length();
    }

    inline Vector3d rotate(const Vector3d &axis, double angle) const
    {
        double c = cos(angle);
        double s = sin(angle);
        double t = 1 - c;

        Vector3d u = axis.unit();
        double x = u.x();
        double y = u.y();
        double z = u.z();

        double m[3][3] = {
            {t * x * x + c, t * x * y - z * s, t * x * z + y * s},
            {t * x * y + z * s, t * y * y + c, t * y * z - x * s},
            {t * x * z - y * s, t * y * z + x * s, t * z * z + c}};

        return Vector3d(m[0][0] * e[0] + m[0][1] * e[1] + m[0][2] * e[2],
                        m[1][0] * e[0] + m[1][1] * e[1] + m[1][2] * e[2],
                        m[2][0] * e[0] + m[2][1] * e[1] + m[2][2] * e[2]);
    }
};