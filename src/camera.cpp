#include "camera.hpp"
#include "color.hpp"
#include "direction.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "point.hpp"
#include "random_generator.hpp"
#include "vector3d.hpp"
#include "sphere.hpp"
#include <iostream>

Camera::Camera(double aspect_ratio, int image_width) : aspect_ratio(aspect_ratio), image_width(image_width)
{
    image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    focal_length = 1.0;
    viewport_height = 2.0;
    viewport_width = aspect_ratio * viewport_height;
    center = Pointer(0, 0, 0);

    viewport_u = Direction(viewport_width, 0, 0);
    viewport_v = Direction(0, -viewport_height, 0);

    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    viewport_upper_left = center - Direction(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;

    viewport_lower_left = center - Direction(0, 0, focal_length) + viewport_u / 2 + viewport_v / 2;

    pixel00_center = viewport_upper_left + pixel_delta_u / 2 + pixel_delta_v / 2;

    image = Image(image_width, image_height);
};

void Camera::translate(const Direction &translation)
{
    center = center + translation;
    viewport_upper_left = center - Direction(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    viewport_lower_left = center - Direction(0, 0, focal_length) + viewport_u / 2 + viewport_v / 2;
    pixel00_center = viewport_upper_left + pixel_delta_u / 2 + pixel_delta_v / 2;
    std::clog << "Camera position: " << center.x() << ", " << center.y() << ", " << center.z() << '\n';
}

void Camera::set_direction(const Direction &direction)
{
    viewport_upper_left = center - Direction(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    viewport_lower_left = center - Direction(0, 0, focal_length) + viewport_u / 2 + viewport_v / 2;
    pixel00_center = viewport_upper_left + pixel_delta_u / 2 + pixel_delta_v / 2;
}

void Camera::render()
{
    HittableList world;
    auto material1 = std::make_shared<Lambertian>(Color(125, 125, 125));
    auto material2 = std::make_shared<Lambertian>(Color(125,0,0));
    auto material3 = std::make_shared<Lambertian>(Color(0, 125, 0));
    auto material4 
        = std::make_shared<Metal>(Color(255, 255, 255), 0.1);
    auto material5 
        = std::make_shared<Dielectric>(1.5);
    auto sphere1 
        = std::make_shared<Sphere>(Pointer(0, -100000, -1), 99999.5, material1);
    auto sphere2 
        = std::make_shared<Sphere>(Pointer(0, 0, -2), 0.5, material2);
    auto sphere3 
        = std::make_shared<Sphere>(Pointer(-2, 0, -2), 0.5, material3);
    auto sphere4 
        = std::make_shared<Sphere>(Pointer(2, 0, -2), 0.5, material4);
    auto sphere5 
        = std::make_shared<Sphere>(Pointer(-0.5, 0, -1), 0.5, material5);
    world.add(sphere1);
    world.add(sphere2);
    world.add(sphere3);
    world.add(sphere4);
    world.add(sphere5);
    for (int j = 0; j < image_height; ++j)
    {
        //std::clog << "Scanlines remaining: " << image_height - j << '\n';
        for (int i = 0; i < image_width; ++i)
        {
            auto p = pixel00_center + pixel_delta_u * i + pixel_delta_v * j;

            Color pixel_color(0, 0, 0);

            for (int s = 0; s < samples_per_pixel; ++s)
            {
                auto random_point = random_generator.sample_point_square(p, 0.01, RandomGenerator::Z);
                Direction direction = Direction(random_point - center).unit();
                Ray ray(center, direction);

                pixel_color = pixel_color + ray_color(ray, max_depth, world);
            }
            pixel_color = pixel_color / samples_per_pixel;
            image.set_pixel(i, j, pixel_color);
        }
    }
}

Color Camera::ray_color(const Ray &ray, int depth, const Hittable &world)
{
    if (depth <= 0)
    {
        return Color(0, 0, 255);
    }

    HitRecord rec;

    if (world.hit(ray, 0.001, 1000, rec))
    {
        ScatterRecord srec;
        rec.material->scatter(ray, rec, srec);
        return srec.attenuation * ray_color(srec.scattered_ray, depth - 1, world) / Color(255,255,255) + srec.emitted;
    } else {
        Color background = Color(255, 255, 255);
        return background;
    }
}


void Camera::write_image(std::ostream &out) const
{
    image.write_as_ppm(out);
}