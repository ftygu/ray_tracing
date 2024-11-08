#include "triangle.hpp"

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

AABB Triangle::bounding_box() const
{
    auto min_x = std::min(v0.x(), std::min(v1.x(), v2.x()));
    auto min_y = std::min(v0.y(), std::min(v1.y(), v2.y()));
    auto min_z = std::min(v0.z(), std::min(v1.z(), v2.z()));

    auto max_x = std::max(v0.x(), std::max(v1.x(), v2.x()));
    auto max_y = std::max(v0.y(), std::max(v1.y(), v2.y()));
    auto max_z = std::max(v0.z(), std::max(v1.z(), v2.z()));

    return AABB(Point(min_x, min_y, min_z), Point(max_x, max_y, max_z));
}