#include "camera.hpp"
#include "basic_types.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "pdf.h"
#include "photo_map.hpp"
#include "random_generator.hpp"
#include "sphere.hpp"
#include "triangle.hpp"
#include <cstddef>
#include <iostream>
#include <omp.h>
#include <vector>

Camera::Camera(double aspect_ratio, int image_width, int samples_per_pixel, int max_depth, Point center)
    : aspect_ratio(aspect_ratio), image_width(image_width), samples_per_pixel(samples_per_pixel), max_depth(max_depth), center(center)
{
    image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;
    image = Image(image_width, image_height);

    //maybe change these to be parameters
    focal_length = 1.0;
    viewport_height = 2.0;
    viewport_width = aspect_ratio * viewport_height;

    //一个像素的边长
    //只支持正方形像素
    pixel_lenght = viewport_width / image_width;

    center = Point(0, 1, 0);
    forward = Direction(0, 0, -1);
    up = Direction(0, 1, 0);
    right = Direction(1, 0, 0);

    update_viewport();
};

void Camera::update_viewport()
{
    viewport_u = right * viewport_width;
    viewport_v = (Direction(0, 0, 0) - up) * viewport_height;

    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    viewport_upper_left = center + forward * focal_length - viewport_u / 2 - viewport_v / 2;
    viewport_lower_left = center + forward * focal_length + viewport_u / 2 + viewport_v / 2;

    pixel00_center = viewport_upper_left + pixel_delta_u / 2 + pixel_delta_v / 2;
}

void Camera::translate(const Direction &translation)
{
    center = center + translation;
    update_viewport();
}

void Camera::translate(double forward, double right, double up)
{
    center = center + this->forward * forward + this->right * right + this->up * up;
    update_viewport();
}


void Camera::rotate(double yaw, double pitch, double roll)
{
    if (yaw != 0)
    {
        forward = forward.rotate(up, yaw);
        right = right.rotate(up, yaw);
    } else if (pitch != 0)
    {
        forward = forward.rotate(right, pitch);
        up = up.rotate(right, pitch);
    } else if (roll != 0)
    {
        right = right.rotate(forward, roll);
        up = up.rotate(forward, roll);
    }

    update_viewport();
}

void Camera::set_position(const Point &position)
{
    center = position;
    update_viewport();
}

void Camera::set_direction(const Direction &direction)
{
    forward = direction;
    right = Direction(0, 0, 0) - forward.cross(up);
    up = right.cross(forward);
    update_viewport();
}

void Camera::set_algorithm(int algorithm)
{
    this->algorithm = algorithm;
}

void Camera::set_world(std::shared_ptr<Hittable> world, std::vector<std::shared_ptr<Hittable>> lights)
{
    this->world = world;
    this->lights = lights;
}

void Camera::render()
{
    for (int j = 0; j < image_height; ++j)
    {
        for (int i = 0; i < image_width; ++i)
        {
            //该像素的中心点坐标
            auto p = pixel00_center + pixel_delta_u * i + pixel_delta_v * j;

            //该像素的颜色，初始化为黑色
            Color pixel_color(0, 0, 0);

            for (int s = 0; s < samples_per_pixel; ++s)
            {
                //在像素内随机采样一个点
                auto random_point = random_generator.sample_point_square(p, pixel_lenght, viewport_u, viewport_v);

                //光线的方向就是从相机中心指向采样点的方向
                Direction direction = Direction(random_point - center).unit();

                //生成光线
                Ray ray(center, direction);

                //像素的颜色就是所有采样点的颜色的平均值
                switch (algorithm)
                {
                    case Algorithm::PathTracing:
                        pixel_color = pixel_color + ray_color(ray, max_depth, *world);
                        break;
                    case Algorithm::PathTracingPDF:
                        pixel_color = pixel_color + ray_color_pdf(ray, max_depth, *world);
                        break;
                    case Algorithm::PhotonMapping:
                        //TODO
                        break;
                    default:
                        break;
                }
            }
            //计算平均值
            pixel_color = pixel_color / samples_per_pixel;

            //将颜色写入图像
            image.set_pixel(i, j, pixel_color);
        }
        std::clog << "Scanlines remaining: " << image_height - j << '\n';
    }
}

//并行渲染，和上面的区别是使用了OpenMP
//基本上一模一样，只是加了#pragma omp parallel for schedule(guided)
void Camera::render_parallel()
{
    int num_threads = omp_get_max_threads();
    omp_set_num_threads(num_threads);

    for (int j = 0; j < image_height; ++j)
    {
        #pragma omp parallel for schedule(guided)
        for (int i = 0; i < image_width; ++i)
        {
            auto p = pixel00_center + pixel_delta_u * i + pixel_delta_v * j;
            Color pixel_color(0, 0, 0);

            for (int s = 0; s < samples_per_pixel; ++s)
            {
                auto random_point = random_generator.sample_point_square(p, pixel_lenght, viewport_u, viewport_v);
                Direction direction = Direction(random_point - center).unit();
                Ray ray(center, direction);
                switch (algorithm)
                {
                    case Algorithm::PathTracing:
                        pixel_color = pixel_color + ray_color(ray, max_depth, *world);
                        break;
                    case Algorithm::PathTracingPDF:
                        pixel_color = pixel_color + ray_color_pdf(ray, max_depth, *world);
                        break;
                    case Algorithm::PhotonMapping:
                        //TODO
                        break;
                    default:
                        break;
                }
            }
            pixel_color = pixel_color / samples_per_pixel;
            image.set_pixel(i, j, pixel_color);
        }
        std::clog << "Scanlines remaining: " << image_height - j << '\n';
    }
}

//通过路径追踪来计算像素的颜色
//每次碰撞后，根据材质的散射函数随即获取一个散射光线
Color Camera::ray_color(const Ray &ray, int depth, const Hittable &world)
{
    if (depth <= 0)
    {
        //如果递归深度为0，返回黑色
        return overflows_color;
    }

    HitRecord rec;

    if (world.hit(ray, 0.001, 1000, rec))
    {
        //如果碰撞，计算碰撞点是如何散射的光线
        ScatterRecord srec;
        rec.material->scatter(ray, rec, srec);

        //像素的颜色 = 碰撞点发出的光线的颜色 + 碰撞点反射的光线的颜色 * 碰撞点的颜色衰减
        return srec.attenuation * ray_color(srec.scattered_ray, depth - 1, world) / Color(255,255,255) + srec.emitted;
    } else {

        //如果没有碰撞，返回背景色
        Color background = Color(100, 100, 100);
        return background;
    }
}

//通过pdf来计算像素的颜色
//与ray_color的区别是，我们只根据材料的特性来随即生成散射光线
//而是根据pdf来生成散射光线，也就是重要性采样
Color Camera::ray_color_pdf(const Ray &ray, int depth, const Hittable &world)
{
    if (depth <= 0)
    {
        return overflows_color;
    }

    HitRecord rec;

    if (world.hit(ray, 0.001, 1000, rec))
    {
        auto srec = ScatterRecord();
        rec.material->scatter(ray, rec, srec);   

        //在pdf采样中，我们放弃了记录光源之间的光照。
        //因为光源可能会对自己采样，会导致光线与自己相交。
        if (srec.emitted.r() > 0 || srec.emitted.g() > 0 || srec.emitted.b() > 0) {
            return srec.emitted;
        }

        //计算所有光源的pdf
        std::vector<PDF*> light_pdfs;
        std::vector<double> light_weights;

        for (const auto& light : lights) {
            double weights = 1.0 / lights.size();
            light_pdfs.push_back(new HittablePDF(rec.p, light));
            light_weights.push_back(weights);
        }

        MixturePDF light_pdf(light_pdfs, light_weights);

        //计算最终的pdf
        std::vector<PDF*> pdfs;
        std::vector<double> weights;
        pdfs.push_back(new SpherePDF());
        pdfs.push_back(&light_pdf);
        weights.push_back(0.5);
        weights.push_back(0.5);

        MixturePDF mixture_pdf(pdfs, weights);

        auto scattered_direction = mixture_pdf.generate(random_generator);

        Ray scattered_ray(rec.p, scattered_direction);

        double pdf_value = mixture_pdf.value(scattered_direction);

        //这里存在神秘的浮点误差
        //比方说我们生成了光源边缘的点，再计算pdf时，虽然这点在光源上，但是pdf值可能为0
        //求交时的误差可能导致这种情况
        while (pdf_value == 0) {
            scattered_direction = mixture_pdf.generate(random_generator);
            scattered_ray = Ray(rec.p, scattered_direction);
            pdf_value = mixture_pdf.value(scattered_direction);
        }

        double scattering_pdf = rec.material->scattering_pdf(ray, rec, scattered_ray);

        Color sample_color = ray_color_pdf(scattered_ray, depth - 1, world);

        //重要性采样
        Color result = srec.attenuation * sample_color / Color(255, 255, 255) * scattering_pdf / pdf_value + srec.emitted;

        return result;
    } else {

        //如果没有碰撞，返回背景色
        return background_color;

    }
}

//通过光子映射来计算像素的颜色
Color Camera::ray_color_photons(const Ray &ray, int depth, const Hittable &world, Photomap &photomap) {

    if (depth <= 0) {
        return overflows_color;
    }

    HitRecord rec;

    if (world.hit(ray, 0.001, 1000, rec)) {

        ScatterRecord srec;

        rec.material->scatter(ray, rec, srec);

        int num_photons = 100;

        std::vector<Photon> nearby_photons = photomap.get_nearby_photons(rec.p, num_photons);

        auto radius = (nearby_photons.back().position - rec.p).length();


        Color indirect_illumination = Color(0, 0, 0);

        for (const Photon& photon : nearby_photons) {

            Direction photon_incoming_direction = -photon.direction;

            double cosine = rec.normal.dot(photon_incoming_direction);

            if (cosine > 0) {

                indirect_illumination = indirect_illumination + photon.power * cosine / 10000000;

            }
        }

        indirect_illumination = indirect_illumination / (M_PI * radius * radius);

        Color direct_illumination = srec.emitted;

        return direct_illumination + srec.attenuation * indirect_illumination;

    } else {

        return background_color;

    }
}



void Camera::write_image(std::ostream &out) const
{
    image.write_as_ppm(out);
}