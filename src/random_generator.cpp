#include "random_generator.hpp"

double RandomGenerator::get_random_double(double min, double max)
{
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

Point RandomGenerator::sample_point_square(Point center, double side_length, Direction u_direction, Direction v_direction)
{
    double half_side_length = side_length / 2;
    
    double rand_u = get_random_double(-half_side_length, half_side_length);
    double rand_v = get_random_double(-half_side_length, half_side_length);

    double x = center.x() + rand_u * u_direction.x() + rand_v * v_direction.x();
    double y = center.y() + rand_u * u_direction.y() + rand_v * v_direction.y();
    double z = center.z() + rand_u * u_direction.z() + rand_v * v_direction.z();

    return Point(x, y, z);
}

Point RandomGenerator::sample_point_sphere(Point center, double radius)
{
    std::uniform_real_distribution<> dis(0, 2 * M_PI);
    double theta = dis(gen);

    dis = std::uniform_real_distribution<>(0, M_PI);
    double phi = dis(gen);

    double x = center.x() + radius * sin(phi) * cos(theta);
    double y = center.y() + radius * sin(phi) * sin(theta);
    double z = center.z() + radius * cos(phi);

    return Point(x, y, z);
}

Direction RandomGenerator::sample_direction_sphere()
{
    std::uniform_real_distribution<> dis(0, 2 * M_PI);
    double theta = dis(gen);

    dis = std::uniform_real_distribution<>(0, M_PI);
    double phi = dis(gen);

    double x = sin(phi) * cos(theta);
    double y = sin(phi) * sin(theta);
    double z = cos(phi);

    return Direction(x, y, z);
}

Point RandomGenerator::sample_point_sphere_surface(Point center, double radius)
{
    std::uniform_real_distribution<> dis(0, 2 * M_PI);
    double theta = dis(gen);

    dis = std::uniform_real_distribution<>(0, M_PI);
    double phi = dis(gen);

    double x = center.x() + radius * sin(phi) * cos(theta);
    double y = center.y() + radius * sin(phi) * sin(theta);
    double z = center.z() + radius * cos(phi);

    return Point(x, y, z);
}