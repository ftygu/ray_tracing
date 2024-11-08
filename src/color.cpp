#include "color.hpp"
#include <cmath>

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
    const double gamma = 2.2;

    auto gamma_correct = [&](double c) -> int {
        double normalized = c / 255.0;
        double corrected = std::pow(normalized, 1.0 / gamma);
        return static_cast<int>(corrected * 255.0 + 0.5);
    };
    
    int ir = gamma_correct(r());
    int ig = gamma_correct(g());
    int ib = gamma_correct(b());
    
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