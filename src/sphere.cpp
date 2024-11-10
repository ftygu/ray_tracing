#include "sphere.hpp"
#include "basic_types.hpp"
#include "hittable.hpp"
#include "random_generator.hpp"

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

double Sphere::pdf_value(const Point &o, const Direction &v) const
{
    HitRecord rec;

    if (!this->hit(Ray(o, v), 0.001, std::numeric_limits<double>::infinity(), rec))
    {
        return 0;
    }

    auto cos_theta_max = std::sqrt(1 - radius * radius / (center - o).length_squared());
    auto solid_angle = 2 * M_PI * (1 - cos_theta_max);

    return 1 / solid_angle;
}

Point Sphere::random(RandomGenerator &random_generator) const
{
    return random_generator.sample_point_sphere(center, radius);
}