#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable {
public:
    sphere() : radius(0.0) {}
    // Stationary Sphere
    sphere(const point3& static_center, double radius, shared_ptr<material> mat)
        : center(static_center, vec3(0, 0, 0)), radius(std::fmax(0, radius)), mat(mat) {}

    // Moving Sphere
    sphere(const point3& center1, const point3& center2, double radius,
        shared_ptr<material> mat)
        : center(center1, center2 - center1), radius(std::fmax(0, radius)), mat(mat) {}

    virtual bool hit(
        const ray& r, interval ray_t, hit_record& rec) const override;

private:
    ray center;

    double radius;
    shared_ptr<material> mat;
};

bool sphere::hit(const ray& r, interval ray_t, hit_record& rec) const {
    point3 current_center = center.at(r.time());
    vec3 oc = current_center - r.origin();
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (!ray_t.surrounds(root)) {
        root = (-half_b + sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            return false;
        }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - current_center) / radius;    rec.set_face_normal(r, outward_normal);

    rec.mat = mat;

    return true;
}


#endif