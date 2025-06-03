#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"           // defines 'ray'
#include "Vector.h"        // defines 'vec3', 'point3', and 'dot(...)'
#include "Interval.h"      // defines 'interval'
#include <memory>          // defines std::shared_ptr
#include "aabb.h"

using std::shared_ptr;


class material;

class hit_record {
public:
    point3 p;
    vec3 normal;
    double t;
    shared_ptr<material> mat;
    bool front_face;

    void set_face_normal(const ray& r, const vec3& outward_normal) {
        // Sets the hit record normal vector.
        // NOTE: the parameter `outward_normal` is assumed to have unit length.

        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual ~hittable() = default;


    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;

    virtual aabb bounding_box() const = 0;

};

#endif