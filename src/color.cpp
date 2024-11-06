#include "color.hpp"

Color::Color() : vector(Vector3d()) {}

Color::Color(double r, double g, double b) : vector(Vector3d(r, g, b)) {}

Color::Color(const Vector3d &v) : vector(v) {}

double Color::r() const
{
    return vector[0];
}

double Color::g() const
{
    return vector[1];
}

double Color::b() const
{
    return vector[2];
}

void Color::write_as_ppm(std::ostream &out) const
{
    int ir = static_cast<int>(r());
    int ig = static_cast<int>(g());
    int ib = static_cast<int>(b());
    out << ir << ' ' << ig << ' ' << ib << '\n';
}

Color Color::operator+(const Color &c) const
{
    return Color(vector + c.vector);
}

Color Color::operator-(const Color &c) const
{
    return Color(vector - c.vector);
}

Color Color::operator*(const Color &c) const
{
    return Color(vector * c.vector);
}

Color Color::operator/(const Color &c) const
{
    return Color(vector / c.vector);
}

Color Color::operator*(double t) const
{
    return Color(vector * t);
}

Color Color::operator/(double t) const
{
    return Color(vector / t);
}

Color Color::operator+(double t) const
{
    return Color(vector + t);
}

Color Color::operator-(double t) const
{
    return Color(vector - t);
}