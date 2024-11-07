#pragma once

#include "hittable.hpp"
#include "material.hpp"
#include "point.hpp"
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
};