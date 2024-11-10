#pragma once

#include "vector3d.hpp"

class Direction;

class Point
{
private:
    Vector3d vector;
public:
    Point() : vector(Vector3d()) {}
    Point(double x, double y, double z) : vector(Vector3d(x, y, z)) {}
    Point(const Vector3d &v) : vector(v) {}

    inline double x() const;

    inline double y() const;

    inline double z() const;

    inline const Vector3d get_vector() const;

    inline double operator[](int i) const;

    inline const Point operator+(const Direction &d) const;

    inline const Point operator-(const Direction &d) const;

    inline const Direction operator-(const Point &p) const;
};

class Direction
{
private:

    Vector3d vector;

public:

    Direction() : vector(Vector3d()) {}
    Direction(double x, double y, double z) : vector(Vector3d(x, y, z)) {}
    Direction(const Vector3d &v) : vector(v) {}

    inline double x() const;

    inline double y() const;

    inline double z() const;

    inline const Vector3d get_vector() const;

    inline const Direction unit() const;

    inline float length() const;

    inline float length_squared() const;

    inline float dot(const Direction &d) const;

    inline const Direction cross(const Direction &d) const;

    inline const Direction operator+() const;

    inline const Direction operator-() const;

    inline const Direction operator+(const Direction &d) const;

    inline const Direction operator-(const Direction &d) const;

    inline const Point operator+(const Point &p) const;

    inline const Point operator-(const Point &p) const;

    inline const Direction operator*(double t) const;

    inline const Direction operator/(double t) const;

    inline const Direction rotate(const Direction &axis, double angle) const;
};

class Color
{
private:

    Vector3d vector;

public:
    Color() : vector(Vector3d()) {}
    Color(double r, double g, double b) : vector(Vector3d(r, g, b)) {}
    Color(const Vector3d &v) : vector(v) {}

    inline double r() const;

    inline double g() const;

    inline double b() const;

    inline void write_as_ppm(std::ostream &out) const;

    inline const Color operator+(const Color &c) const;
    inline const Color operator-(const Color &c) const;
    inline const Color operator*(const Color &c) const;
    inline const Color operator/(const Color &c) const;

    inline const Color operator*(double t) const;
    inline const Color operator/(double t) const;
    inline const Color operator+(double t) const;
    inline const Color operator-(double t) const;
};

// Point Part
    inline double Point::x() const
    {
        return vector[0];
    }

    inline double Point::y() const
    {
        return vector[1];
    }

    inline double Point::z() const
    {
        return vector[2];
    }

    inline const Vector3d Point::get_vector() const
    {
        return vector;
    }

    inline double Point::operator[](int i) const
    {
        return vector[i];
    }

    inline const Point Point::operator+(const Direction &d) const
    {
        return Point(vector + d.get_vector());
    }

    inline const Point Point::operator-(const Direction &d) const
    {
        return Point(vector - d.get_vector());
    }

    inline const Direction Point::operator-(const Point &p) const
    {
        return Direction(vector - p.get_vector());
    }


// Direction Part
    inline double Direction::x() const
    {
        return vector[0];
    }

    inline double Direction::y() const
    {
        return vector[1];
    }

    inline double Direction::z() const
    {
        return vector[2];
    }

    inline const Vector3d Direction::get_vector() const
    {
        return vector;
    }

    inline const Direction Direction::unit() const
    {
        Vector3d unit_vector = vector.unit();
        return Direction(unit_vector);
    }

    inline float Direction::length() const
    {
        return vector.length();
    }

    inline float Direction::length_squared() const
    {
        return vector.length_squared();
    }

    inline float Direction::dot(const Direction &d) const
    {
        return vector.dot(d.vector);
    }

    inline const Direction Direction::cross(const Direction &d) const
    {
        Vector3d cross_vector = vector.cross(d.vector);
        return Direction(cross_vector);
    }

    inline const Direction Direction::operator+() const
    {
        return *this;
    }

    inline const Direction Direction::operator-() const
    {
        return Direction(-vector);
    }

    inline const Direction Direction::operator+(const Direction &d) const
    {
        return Direction(vector + d.vector);
    }

    inline const Direction Direction::operator-(const Direction &d) const
    {
        return Direction(vector - d.vector);
    }

    inline const Point Direction::operator+(const Point &p) const
    {
        return Point(vector + p.get_vector());
    }

    inline const Point Direction::operator-(const Point &p) const
    {
        return Point(vector - p.get_vector());
    }

    inline const Direction Direction::operator*(double t) const
    {
        return Direction(vector * t);
    }

    inline const Direction Direction::operator/(double t) const
    {
        return Direction(vector / t);
    }

    inline const Direction Direction::rotate(const Direction &axis, double angle) const
    {
        double c = cos(angle);
        double s = sin(angle);
        double t = 1 - c;

        double x = vector.x();
        double y = vector.y();
        double z = vector.z();

        double a = axis.x();
        double b = axis.y();
        double c1 = axis.z();

        double x_rot = (t * a * a + c) * x + (t * a * b - s * c1) * y + (t * a * c1 + s * b) * z;
        double y_rot = (t * a * b + s * c1) * x + (t * b * b + c) * y + (t * b * c1 - s * a) * z;
        double z_rot = (t * a * c1 - s * b) * x + (t * b * c1 + s * a) * y + (t * c1 * c1 + c) * z;

        return Direction(x_rot, y_rot, z_rot);
    }

// Color Part
    inline double Color::r() const
    {
        return vector[0];
    }

    inline double Color::g() const
    {
        return vector[1];
    }

    inline double Color::b() const
    {
        return vector[2];
    }

    inline void Color::write_as_ppm(std::ostream &out) const
    {
        const double gamma = 2.2;

        auto gamma_correct = [&](double c) -> int {
            double normalized = c / 255.0;
            double corrected = std::pow(normalized, 1.0 / gamma);
            return static_cast<int>(corrected * 255.0 + 0.5);
        };
        
        int ir = gamma_correct(r());
        int ig = gamma_correct(g());
        int ib = gamma_correct(b());

        if (ir > 255) ir = 255;
        if (ig > 255) ig = 255;
        if (ib > 255) ib = 255;
        if (ir < 0) ir = 0;
        if (ig < 0) ig = 0;
        if (ib < 0) ib = 0;
        
        out << ir << ' ' << ig << ' ' << ib << '\n';
    }

    inline const Color Color::operator+(const Color &c) const
    {
        return Color(vector + c.vector);
    }

    inline const Color Color::operator-(const Color &c) const
    {
        return Color(vector - c.vector);
    }

    inline const Color Color::operator*(const Color &c) const
    {
        return Color(vector * c.vector);
    }

    inline const Color Color::operator/(const Color &c) const
    {
        return Color(vector / c.vector);
    }

    inline const Color Color::operator*(double t) const
    {
        return Color(vector * t);
    }

    inline const Color Color::operator/(double t) const
    {
        return Color(vector / t);
    }

    inline const Color Color::operator+(double t) const
    {
        return Color(vector + t);
    }

    inline const Color Color::operator-(double t) const
    {
        return Color(vector - t);
    }