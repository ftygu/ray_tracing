#pragma once

#include "basic_types.hpp"
#include "ray.hpp"
#include "hittable.hpp"

#include "random_generator.hpp"
#include <sstream>

class ScatterRecord{
public:
    Ray scattered_ray;
    Color attenuation;
    Color emitted;
};

class Material
{
protected:
    static RandomGenerator random_generator;
    Color light_color;
public:
    virtual void scatter(const Ray &ray_in, const HitRecord &rec, ScatterRecord &srec) const = 0;

    //负责重要性采样时的pdf计算
    virtual double scattering_pdf(const Ray &ray_in, const HitRecord &rec, const Ray &scattered) const
    {
        return 0;
    }
    
    void set_light_color(const Color &light_color);
};

class Lambertian : public Material
{
private:
    Color albedo;
public:
    Lambertian(const Color &albedo) : albedo(albedo) {}
    virtual void scatter(const Ray &ray_in, const HitRecord &rec, ScatterRecord &srec) const override;
    virtual double scattering_pdf(const Ray &ray_in, const HitRecord &rec, const Ray &scattered) const override;
};

class Metal : public Material
{
private:
    Color albedo;
    double fuzz;
    static Direction reflect(const Direction &v, const Direction &n);
public:
    Metal(const Color &albedo, double fuzz) : albedo(albedo), fuzz(fuzz) {}
    virtual void scatter(const Ray &ray_in, const HitRecord &rec, ScatterRecord &srec) const override;
};

class Dielectric : public Material
{
private:
    double refraction_index;
    static Direction reflect(const Direction &v, const Direction &n);
    static bool refract(const Direction &v, const Direction &n, double ni_over_nt, Direction &refracted);
    static double schlick(double cosine, double refraction_index);
public:
    Dielectric(double refraction_index) : refraction_index(refraction_index) {}
    virtual void scatter(const Ray &ray_in, const HitRecord &rec, ScatterRecord &srec) const override;
};