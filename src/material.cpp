#include "material.hpp"
#include "basic_types.hpp"
#include "ray.hpp"

RandomGenerator Material::random_generator;

void Material::set_light_color(const Color &light_color)
{
    this->light_color = light_color;
}

void Lambertian::scatter(const Ray &ray_in, const HitRecord &rec, ScatterRecord &srec) const
{
    auto random_point = random_generator.sample_point_sphere(rec.p, 1) + rec.normal;
    auto ray_direction = Direction(random_point - rec.p).unit();
    srec.scattered_ray = Ray(rec.p, ray_direction);
    srec.attenuation = albedo;
    srec.emitted = light_color;
}

double Lambertian::scattering_pdf(const Ray &ray_in, const HitRecord &rec, const Ray &scattered) const
{
    auto cosine = rec.normal.dot(scattered.get_direction());

    if (cosine < 0)
    {
        cosine = 0;
    }

    return cosine / M_PI;
}

Direction Metal::reflect(const Direction &v, const Direction &n)
{
    return v - n * v.dot(n) * 2;
}

void Metal::scatter(const Ray &ray_in, const HitRecord &rec, ScatterRecord &srec) const
{
    auto random_point = random_generator.sample_point_sphere(rec.p, fuzz) + rec.normal;
    auto ray_direction = Direction(random_point - rec.p).unit();
    srec.scattered_ray = Ray(rec.p, ray_direction);
    srec.attenuation = albedo;
    srec.emitted = light_color;
}

Direction Dielectric::reflect(const Direction& v, const Direction& n)
{
    return v - n * v.dot(n) * 2;
}

bool Dielectric::refract(const Direction& v, const Direction& n, double ni_over_nt, Direction& refracted)
{
    Direction uv = v.unit();
    double dt = uv.dot(n);
    double discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
    if (discriminant > 0)
    {
        refracted = (uv - n * dt) * ni_over_nt - n * sqrt(discriminant);
        return true;
    }
    else
    {
        return false;
    }
}

double Dielectric::schlick(double cosine, double ref_idx)
{
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow(1 - cosine, 5);
}


void Dielectric::scatter(const Ray &ray_in, const HitRecord &rec, ScatterRecord &srec) const
{
    srec.attenuation = Color(255, 255, 255);
    srec.emitted = light_color;

    Direction reflected = reflect(ray_in.get_direction(), rec.normal);
    double ni_over_nt;
    Direction outward_normal;
    double cosine;
    double reflect_prob;
    Direction refracted;

    if (ray_in.get_direction().dot(rec.normal) > 0)
    {
        outward_normal = Direction(0, 0, 0) - rec.normal;
        ni_over_nt = refraction_index;
        cosine = refraction_index * ray_in.get_direction().dot(rec.normal) / ray_in.get_direction().length();
    }
    else
    {
        outward_normal = rec.normal;
        ni_over_nt = 1.0 / refraction_index;
        cosine = -ray_in.get_direction().dot(rec.normal) / ray_in.get_direction().length();
    }

    if (refract(ray_in.get_direction(), outward_normal, ni_over_nt, refracted))
    {
        reflect_prob = schlick(cosine, refraction_index);
    }
    else
    {
        reflect_prob = 1.0;
    }

    if (random_generator.get_random_double(0, 1) < reflect_prob)
    {
        srec.scattered_ray = Ray(rec.p, reflected);
    }
    else
    {
        srec.scattered_ray = Ray(rec.p, refracted);
    }
}