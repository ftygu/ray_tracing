#pragma once

#include "direction.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "image.hpp"
#include "color.hpp"
#include "random_generator.hpp"
#include "ray.hpp"
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

    Point center;


    Direction forward;
    Direction up;
    Direction right;

    Direction viewport_u;
    Direction viewport_v;

    Direction pixel_delta_u;
    Direction pixel_delta_v;

    Point viewport_upper_left;
    Point viewport_lower_left;

    Point pixel00_center;

    Image image;

    int samples_per_pixel = 10;
    int max_depth = 25;

    RandomGenerator random_generator;

    HittableList world;
public:

    Camera(double aspect_ratio, int image_width);

    void translate(const Direction &translation);

    void translate(double forward, double right, double up);

    Direction rotate_vector(const Direction& v, const Direction& axis, double angle);

    void rotate(double yaw, double pitch, double roll);

    void set_position(const Point &position);

    void set_direction(const Direction &direction);

    void set_world(const HittableList &world);

    void render();

    Color ray_color(const Ray &ray, int depth, const Hittable &world);

    void write_image(std::ostream &out) const;
};