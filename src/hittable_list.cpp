#include "hittable_list.hpp"

void HittableList::add(std::shared_ptr<Hittable> object)
{
    objects.push_back(object);
}

bool HittableList::hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const
{
    HitRecord temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;

    for (const auto &object : objects)
    {
        if (object->hit(ray, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

AABB HittableList::bounding_box() const
{
    if (objects.empty())
    {
        return AABB(Point(0, 0, 0), Point(0, 0, 0));
    }

    AABB box;
    bool first_box = true;

    for (const auto &object : objects)
    {
        auto object_box = object->bounding_box();
        if (first_box)
        {
            box = object_box;
        }
        else
        {
            box = AABB::surrounding_box(box, object_box);
        }
        first_box = false;
    }

    return box;
}