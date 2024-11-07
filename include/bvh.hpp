// bvh.hpp
#pragma once
#include "hittable.hpp"
#include "aabb.hpp"
#include <algorithm>
#include <vector>

class BVH : public Hittable {
public:
    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    AABB box;

    BVH(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end) {
        auto axis = rand() % 3;
        
        auto comparator = (axis == 0) ? box_x_compare
                       : (axis == 1) ? box_y_compare
                                   : box_z_compare;

        size_t object_span = end - start;

        if (object_span == 1) {
            left = right = objects[start];
        } else if (object_span == 2) {
            if (comparator(objects[start], objects[start+1])) {
                left = objects[start];
                right = objects[start+1];
            } else {
                left = objects[start+1];
                right = objects[start];
            }
        } else {
            std::sort(objects.begin() + start, objects.begin() + end, comparator);

            auto mid = start + object_span/2;
            left = std::make_shared<BVH>(objects, start, mid);
            right = std::make_shared<BVH>(objects, mid, end);
        }

        AABB box_left = left->bounding_box();
        AABB box_right = right->bounding_box();

        box = AABB::surrounding_box(box_left, box_right);
    }

    virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override {
        if (!box.hit(ray, t_min, t_max))
        {
            return false;
        }

        bool hit_left = left->hit(ray, t_min, t_max, rec);
        bool hit_right = right->hit(ray, t_min, hit_left ? rec.t : t_max, rec);
        
        return hit_left || hit_right;
    }

    virtual AABB bounding_box() const override {
        return box;
    }

private:

    static bool box_x_compare(const std::shared_ptr<Hittable> a,
                            const std::shared_ptr<Hittable> b) {
        AABB box_a, box_b;
        box_a = a->bounding_box();
        box_b = b->bounding_box();
        return box_a.minimum.x() < box_b.minimum.x();
    }

    static bool box_y_compare(const std::shared_ptr<Hittable> a,
                            const std::shared_ptr<Hittable> b) {
        AABB box_a, box_b;
        box_a = a->bounding_box();
        box_b = b->bounding_box();
        return box_a.minimum.y() < box_b.minimum.y();
    }

    static bool box_z_compare(const std::shared_ptr<Hittable> a,
                            const std::shared_ptr<Hittable> b) {
        AABB box_a, box_b;
        box_a = a->bounding_box();
        box_b = b->bounding_box();
        return box_a.minimum.z() < box_b.minimum.z();
    }
};