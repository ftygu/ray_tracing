#pragma once

#include <random>

#include "basic_types.hpp"

class RandomGenerator
{
public:

    RandomGenerator() : gen(rd()) {}

    double get_random_double(double min, double max);

    //以center为中心，边长为side_length的正方形上随机采样一个点
    Point sample_point_square(Point center, double side_length, Direction u_direction, Direction v_direction);

    Point sample_point_sphere(Point center, double radius);

    Direction sample_direction_sphere();

    Point sample_point_sphere_surface(Point center, double radius);
private:
    std::random_device rd;
    std::mt19937 gen;
};
