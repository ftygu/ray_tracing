#pragma once

#include "hittable.hpp"

#include <memory>
#include <vector>

class HittableList : public Hittable
{
private:

    std::vector<std::shared_ptr<Hittable>> objects;

public:

    HittableList() = default;

    void add(std::shared_ptr<Hittable> object);

    bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override;

    AABB bounding_box() const override;
};