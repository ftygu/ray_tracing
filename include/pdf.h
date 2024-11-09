#pragma once

#include "basic_types.hpp"
#include "random_generator.hpp"

#include <cassert>
#include <memory>

class PDF
{
public:
    virtual ~PDF() = default;
    virtual double value(const Direction &direction) const = 0;
    virtual Direction generate() = 0;
};

class SpherePDF : public PDF
{
private:
    RandomGenerator random_generator;
public:
    SpherePDF() = default;

    double value(const Direction &direction) const override
    {
        return 1.0 / (4.0 * M_PI);
    }

    Direction generate() override
    {
        return random_generator.sample_direction_sphere();
    }

};

class CosinePDF : public PDF
{
private:
    Direction normal;
    double exponent;
    RandomGenerator random_generator;

public:
    explicit CosinePDF(const Direction& n, double exp = 1.0)
        : normal(n.unit()), exponent(exp) {}

    double value(const Direction &direction) const override
    {
        double cosine = normal.dot(direction.unit());
        if (cosine <= 0) return 0;
        return (exponent + 1) * std::pow(cosine, exponent) / (2 * M_PI);
    }

    Direction generate() override
    {
        double r1 = random_generator.get_random_double(0, 1);
        double r2 = random_generator.get_random_double(0, 1);

        double phi = 2 * M_PI * r1;
        double cos_theta = std::pow(r2, 1.0 / (exponent + 1));
        double sin_theta = sqrt(1 - cos_theta * cos_theta);

        double x = sin_theta * cos(phi);
        double y = sin_theta * sin(phi);
        double z = cos_theta;

        Direction w = normal.unit();
        Direction a = (fabs(w.x()) > 0.9) ? Direction(0, 1, 0) : Direction(1, 0, 0);
        Direction v = w.cross(a).unit();
        Direction u = v.cross(w);

        Direction sampled_direction = u * x + v * y + w * z;

        return sampled_direction.unit();
    }
};

class MixturePDF : public PDF
{
private:
    std::shared_ptr<PDF> pdf1;
    std::shared_ptr<PDF> pdf2;
    double weight1;
    double weight2;
    RandomGenerator random_generator;

public:

    MixturePDF(std::shared_ptr<PDF> p1, double weight1, std::shared_ptr<PDF> p2, double weight2)
        : pdf1(p1), pdf2(p2), weight1(weight1), weight2(weight2)
    {
        assert(weight1 + weight2 == 1);
    }

    double value(const Direction &direction) const override
    {
        return weight1 * pdf1->value(direction) + weight2 * pdf2->value(direction);
    }

    Direction generate() override
    {
        if (random_generator.get_random_double(0, 1) < weight1)
        {
            return pdf1->generate();
        }
        else
        {
            return pdf2->generate();
        }
    }

};