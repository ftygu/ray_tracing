#pragma once

#include "basic_types.hpp"
#include "random_generator.hpp"

#include <cassert>
#include <cstdlib>
#include <memory>
#include <vector>

#include "hittable.hpp"

class PDF
{
public:
    virtual ~PDF() = default;
    virtual double value(const Direction &direction) const = 0;
    virtual Direction generate(RandomGenerator &random_generator) = 0;
};

class SpherePDF : public PDF
{
private:
    static constexpr double INV_4PI = 1.0 / (4.0 * M_PI);
public:
    SpherePDF() = default;

    double value(const Direction &direction) const override
    {
        return INV_4PI;
    }

    Direction generate(RandomGenerator &random_generator) override
    {
        return random_generator.sample_direction_sphere();
    }

};

class CosinePDF : public PDF
{
private:
    Direction normal;
    double exponent;

public:
    explicit CosinePDF(const Direction& n, double exp = 1.0)
        : normal(n.unit()), exponent(exp) {}

    double value(const Direction &direction) const override
    {
        double cosine = normal.dot(direction.unit());
        if (cosine <= 0) return 0;
        return (exponent + 1) * std::pow(cosine, exponent) / (2 * M_PI);
    }

    Direction generate(RandomGenerator &random_generator) override
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

class HittablePDF : public PDF
{
private:
    Point origin;
    std::shared_ptr<Hittable> object;

public:

    HittablePDF(const Point &origin, std::shared_ptr<Hittable> object)
        : origin(origin), object(object) {}

    double value(const Direction &direction) const override
    {
        return object->pdf_value(origin, direction);
    }

    Direction generate(RandomGenerator &random_generator) override
    {
        return object->random(random_generator) - origin;
    }

};

class MixturePDF : public PDF
{
private:

    std::vector<PDF*> pdfs;

    std::vector<double> weights;

public:

    MixturePDF(std::vector<PDF*> pdfs, std::vector<double> weights)
        : pdfs(pdfs), weights(weights)
    {
        assert(pdfs.size() == weights.size());
    }

    double value(const Direction &direction) const override
    {
        double sum = 0;
        for (int i = 0; i < pdfs.size(); ++i) {
            sum += weights[i] * pdfs[i]->value(direction);
        }
        return sum;
    }

    Direction generate(RandomGenerator &random_generator) override
    {
        double r = random_generator.get_random_double(0, 1);
        double sum = 0;
        for (int i = 0; i < pdfs.size(); ++i) {
            sum += weights[i];
            if (r < sum) {
                return pdfs[i]->generate(random_generator);
            }
        }
        return pdfs.back()->generate(random_generator);
    }
};