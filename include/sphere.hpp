#include "hittable.hpp"
#include "material.hpp"
#include "basic_types.hpp"
#include "random_generator.hpp"
#include <memory>

class Sphere : public Hittable
{
private:
    Point center;
    double radius;
    std::shared_ptr<Material> material;
public:
    Sphere(const Point &center, double radius, std::shared_ptr<Material> material);

    bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override;

    AABB bounding_box() const override;

    Point get_center() const
    {
        return center;
    }

    double get_radius() const
    {
        return radius;
    }

    double pdf_value(const Point &o, const Direction &v) const override;

    Point random(RandomGenerator &random_generator) const override;
};