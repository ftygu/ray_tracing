#include "random_generator.hpp"
#include "point.hpp"

double RandomGenerator::get_random_double(double min, double max)
{
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

Pointer RandomGenerator::sample_point_square(Pointer center, double side_length, Coordinate fixed_coordinate)
{
    std::uniform_real_distribution<> dis(-side_length / 2, side_length / 2);

    double x = center.x();
    double y = center.y();
    double z = center.z();

    switch (fixed_coordinate)
    {
    case X:
        y += dis(gen);
        z += dis(gen);
        break;
    case Y:
        x += dis(gen);
        z += dis(gen);
        break;
    case Z:
        x += dis(gen);
        y += dis(gen);
        break;
    }

    return Pointer(x, y, z);
}

Pointer RandomGenerator::sample_point_sphere(Pointer center, double radius)
{
    std::uniform_real_distribution<> dis(0, 2 * M_PI);
    double theta = dis(gen);

    dis = std::uniform_real_distribution<>(0, M_PI);
    double phi = dis(gen);

    double x = center.x() + radius * sin(phi) * cos(theta);
    double y = center.y() + radius * sin(phi) * sin(theta);
    double z = center.z() + radius * cos(phi);

    return Pointer(x, y, z);
}