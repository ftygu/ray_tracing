#include "triangle.hpp"
#include "direction.hpp"
#include "point.hpp"
#include "ray.hpp"

Triangle::Triangle(const Point &v0, const Point &v1, const Point &v2, std::shared_ptr<Material> material) : v0(v0), v1(v1), v2(v2), material(material) {}

bool Triangle::hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const
{
    auto edge1 = v1 - v0;
    auto edge2 = v2 - v0;
    auto h = ray.get_direction().cross(edge2);
    auto a = edge1.dot(h);

    if (a > -0.00001 && a < 0.00001)
    {
        return false;
    }

    auto f = 1.0 / a;
    auto s = ray.get_origin() - v0;
    auto u = f * s.dot(h);

    if (u < 0.0 || u > 1.0)
    {
        return false;
    }

    auto q = s.cross(edge1);
    auto v = f * ray.get_direction().dot(q);

    if (v < 0.0 || u + v > 1.0)
    {
        return false;
    }

    auto t = f * edge2.dot(q);

    if (t < t_min || t > t_max)
    {
        return false;
    }

    auto normal = edge1.cross(edge2).unit();
    if (normal.dot(ray.get_direction()) > 0)
    {
        normal = Direction(0, 0, 0) - normal;
    }

    rec.t = t;
    rec.p = ray.at(rec.t);
    rec.normal = normal;
    rec.material = material;

    return true;
}