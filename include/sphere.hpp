#include "hittable.hpp"
#include "material.hpp"
#include "point.hpp"
#include <memory>

class Sphere : public Hittable
{
private:
    Pointer center;
    double radius;
    std::shared_ptr<Material> material;
public:
    Sphere(const Pointer &center, double radius, std::shared_ptr<Material> material);

    bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override;
};