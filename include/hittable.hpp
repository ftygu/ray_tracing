#pragma once

#include "basic_types.hpp"
#include "aabb.hpp"
#include "pdf.h"
#include "ray.hpp"

#include <memory>

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
};