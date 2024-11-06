#pragma once

#include <random>

#include "direction.hpp"
#include "point.hpp"

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

    Pointer sample_point_square(Pointer center, double side_length, Coordinate fixed_coordinate);

    Pointer sample_point_sphere(Pointer center, double radius);
private:
    std::random_device rd;
    std::mt19937 gen;
};
