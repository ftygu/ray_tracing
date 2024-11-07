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


    Direction forward;
    Direction up;
    Direction right;

    Direction viewport_u;
    Direction viewport_v;

    Direction pixel_delta_u;
    Direction pixel_delta_v;

    Pointer viewport_upper_left;
    Pointer viewport_lower_left;

    Pointer pixel00_center;

    Image image;

    int samples_per_pixel = 5;
    int max_depth = 20;

    RandomGenerator random_generator;
public:

    Camera(double aspect_ratio, int image_width);

    void translate(const Direction &translation);

    void translate(double forward, double right, double up);

    Direction rotate_vector(const Direction& v, const Direction& axis, double angle);

    void rotate(double yaw, double pitch, double roll);

    void set_position(const Pointer &position);

    void set_direction(const Direction &direction);

    void render();

    Color ray_color(const Ray &ray, int depth, const Hittable &world);

    void write_image(std::ostream &out) const;
};