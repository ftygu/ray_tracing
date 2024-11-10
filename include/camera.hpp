#pragma once

#include "basic_types.hpp"
#include "pdf.h"
#include "photo_map.hpp"
#include "ray.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"

#include "image.hpp"
#include "random_generator.hpp"

#include <memory>

enum Algorithm
{
    PathTracing,
    PathTracingPDF,
    PhotonMapping
};

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
    double pixel_lenght;

    Image image;

    int samples_per_pixel;
    int max_depth;

    RandomGenerator random_generator;

    std::shared_ptr<Hittable> world;
    Color background_color = Color(0, 0, 0);
    Color overflows_color = Color(255, 255, 0);

    int algorithm = Algorithm::PathTracing;

    std::vector<std::shared_ptr<Hittable>> lights;
private:
    //更新视口， 在相机平移或者旋转后，根据新的相机位置和方向更新视口
    void update_viewport();
public:

    Camera(double aspect_ratio, int image_width, int samples_per_pixel = 100, int max_depth = 10, Point center = Point(0, 1, 0));

    //世界坐标系下的平移
    void translate(const Direction &translation);

    //相机坐标系下的平移
    void translate(double forward, double right, double up);

    //相机坐标系下的旋转
    void rotate(double yaw, double pitch, double roll);

    //设置相机位置
    void set_position(const Point &position);

    //设置相机方向
    void set_direction(const Direction &direction);

    //设置世界
    void set_world(std::shared_ptr<Hittable> world, std::vector<std::shared_ptr<Hittable>> lights);

    //设置算法
    void set_algorithm(int algorithm);

    //渲染
    void render();

    //并行渲染
    void render_parallel();

    //并行渲染pdf
    void render_parallel_pdf();

    //光子映射渲染
    void render_photons(Photomap &photomap);

    //获取像素颜色
    Color ray_color(const Ray &ray, int depth, const Hittable &world);

    //获取像素颜色pdf
    Color ray_color_pdf(const Ray &ray, int depth, const Hittable &world);

    //获取像素颜色光子映射
    Color ray_color_photons(const Ray &ray, int depth, const Hittable &world, Photomap &photomap);

    //以ppm格式写入图像
    void write_image(std::ostream &out) const;
};