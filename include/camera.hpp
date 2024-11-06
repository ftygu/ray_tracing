#pragma once

#include "direction.hpp"
#include "hittable.hpp"
#include "image.hpp"
#include "color.hpp"
#include "random_generator.hpp"
#include "ray.hpp"
#include "vector3d.hpp"
#include "point.hpp"

class Camera
{
private:

    double aspect_ratio;

    int image_width;
    int image_height;

    double focal_length;

    double viewport_height;
    double viewport_width;

    Pointer center;

    Direction viewport_u;
    Direction viewport_v;

    double pixel_delta_u;
    double pixel_delta_v;

    Pointer viewport_upper_left;
    Pointer viewport_lower_left;

    Pointer pixel00_center;

    Image image;

    int samples_per_pixel = 20;
    int max_depth = 100;

    RandomGenerator random_generator;

public:

    Camera(double aspect_ratio, int image_width);

    void translate(const Direction &translation);

    void set_position(const Pointer &position);

    void set_direction(const Direction &direction);

    void render();

    Color ray_color(const Ray &ray, int depth, const Hittable &world);

    void write_image(std::ostream &out) const;
};