#pragma once

#include "hittable.hpp"
#include "material.hpp"
#include "basic_types.hpp"
#include <memory>

class Triangle : public Hittable
{
private:
    Point v0;
    Point v1;
    Point v2;
    std::shared_ptr<Material> material;
public:
    Triangle(const Point &v0, const Point &v1, const Point &v2, std::shared_ptr<Material> material);

    bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override;

    AABB bounding_box() const override;

    double pdf_value(const Point &o, const Direction &v) const override;

    Point random(RandomGenerator &random_generator) const override;
};