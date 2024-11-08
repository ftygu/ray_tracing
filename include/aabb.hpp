// aabb.hpp
#pragma once

#include "ray.hpp"
#include "basic_types.hpp"

class AABB {
public:
    Point minimum;
    Point maximum;

    AABB() {}
    AABB(const Point& a, const Point& b) : minimum(a), maximum(b) {}

    inline bool hit(const Ray& ray, double t_min, double t_max) const {
        auto invD = 1.0f / ray.get_direction().x();
        auto t0 = (minimum.x() - ray.get_origin().x()) * invD;
        auto t1 = (maximum.x() - ray.get_origin().x()) * invD;
        if (invD < 0.0f)
            std::swap(t0, t1);
        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;
        if (t_max <= t_min)
            return false;
        invD = 1.0f / ray.get_direction().y();
        t0 = (minimum.y() - ray.get_origin().y()) * invD;
        t1 = (maximum.y() - ray.get_origin().y()) * invD;
        if (invD < 0.0f)
            std::swap(t0, t1);
        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;
        if (t_max <= t_min)
            return false;
        invD = 1.0f / ray.get_direction().z();
        t0 = (minimum.z() - ray.get_origin().z()) * invD;
        t1 = (maximum.z() - ray.get_origin().z()) * invD;
        if (invD < 0.0f)
            std::swap(t0, t1);
        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;
        if (t_max <= t_min)
        {
            return false;
        }
        return true;
    }

    inline static AABB surrounding_box(const AABB& box0, const AABB& box1) {
        Point small(fmin(box0.minimum.x(), box1.minimum.x()),
                    fmin(box0.minimum.y(), box1.minimum.y()),
                    fmin(box0.minimum.z(), box1.minimum.z()));
        Point big(fmax(box0.maximum.x(), box1.maximum.x()),
                  fmax(box0.maximum.y(), box1.maximum.y()),
                  fmax(box0.maximum.z(), box1.maximum.z()));
        return AABB(small, big);
    }
};