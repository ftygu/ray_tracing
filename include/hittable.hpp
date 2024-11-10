#pragma once

#include "basic_types.hpp"
#include "aabb.hpp"
#include "ray.hpp"

#include <memory>

class RandomGenerator;

class PDF;

class Material;

class HitRecord {
public:
    Point p;
    Direction normal;
    double t;
    std::shared_ptr<Material> material;
    std::shared_ptr<PDF> pdf;
};

class Hittable {
public:
    virtual bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const = 0;

    virtual AABB bounding_box() const = 0;

    virtual double pdf_value(const Point &o, const Direction &v) const 
    {
        return 0.0;
    }

    virtual Point random(RandomGenerator &random_generator) const
    {
        return Point(0, 0, 0);
    }
};