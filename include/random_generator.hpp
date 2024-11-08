#pragma once

#include <random>

#include "basic_types.hpp"

class RandomGenerator
{
public:

    enum Coordinate
    {
        X,
        Y,
        Z
    };

    RandomGenerator() : gen(rd()) {}

    double get_random_double(double min, double max);

    Point sample_point_square(Point center, double side_length, Coordinate fixed_coordinate);

    Point sample_point_sphere(Point center, double radius);
private:
    std::random_device rd;
    std::mt19937 gen;
};
