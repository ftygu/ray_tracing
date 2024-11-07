#include "sphere.hpp"
#include "direction.hpp"
#include "hittable.hpp"

Sphere::Sphere(const Point &center, double radius, std::shared_ptr<Material> material) : center(center), radius(radius), material(material) {}

bool Sphere::hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const
{
    auto oc = ray.get_origin() - center;

    auto a = ray.get_direction().length_squared();
    auto h = (oc.dot(ray.get_direction()));
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = h * h - a * c;

    if (discriminant < 0)
    {
        return false;
    }

    auto sqrtd = std::sqrt(discriminant);
    auto root = (-h - sqrtd) / a;

    if (root < t_min || t_max < root)
    {
        root = (-h + sqrtd) / a;
        if (root < t_min || t_max < root)
        {
            return false;
        }
    }

    rec.t = root;
    rec.p = ray.at(rec.t);
    Direction outward_normal = (rec.p - center) / radius;
    outward_normal = outward_normal.unit();
    rec.normal = outward_normal;
    rec.material = material;

    return true;
}

AABB Sphere::bounding_box() const
{
    return AABB(center - Direction(radius, radius, radius), center + Direction(radius, radius, radius));
}