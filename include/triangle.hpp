#pragma once

#include "hittable.hpp"
#include "material.hpp"
#include "point.hpp"
#include <memory>

class Triangle : public Hittable
{
private:
    Pointer v0;
    Pointer v1;
    Pointer v2;
    std::shared_ptr<Material> material;
public:
    Triangle(const Pointer &v0, const Pointer &v1, const Pointer &v2, std::shared_ptr<Material> material);

    bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override;
};