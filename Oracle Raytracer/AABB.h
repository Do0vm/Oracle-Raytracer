#include "Interval.h"
// Assuming "point3.h" and "ray.h" (which would include "vec3.h") are also included elsewhere or via "Interval.h"
// For point3 and ray definitions. For clarity, explicit includes are better if not transitive.
// #include "point3.h"
// #include "ray.h"

#ifndef AABB_H
#define AABB_H

class aabb {
public:
    interval x, y, z;

    aabb() {} // The default AABB is empty, since intervals are empty by default.

    aabb(const interval& x, const interval& y, const interval& z)
        : x(x), y(y), z(z) {}

    aabb(const point3& a, const point3& b) {
        // Treat the two points a and b as extrema for the bounding box, so we don't require a
        // particular minimum/maximum coordinate order.

        // Assuming interval constructor takes (min, max) and point3 components are accessible via operator[]
        x = (a[0] <= b[0]) ? interval(a[0], b[0]) : interval(b[0], a[0]);
        y = (a[1] <= b[1]) ? interval(a[1], b[1]) : interval(b[1], a[1]);
        z = (a[2] <= b[2]) ? interval(a[2], b[2]) : interval(b[2], a[2]);
    }

    aabb(const aabb& box0, const aabb& box1) {
        // Assuming interval has a constructor that takes two other intervals to form a surrounding interval
        x = interval(box0.x, box1.x);
        y = interval(box0.y, box1.y);
        z = interval(box0.z, box1.z);
    }

    const interval& axis_interval(int n) const {
        if (n == 1) return y;
        if (n == 2) return z;
        return x;
    }

    bool hit(const ray& r, interval ray_t) const {
        const point3& ray_orig = r.origin();
        const vec3& ray_dir = r.direction();

        for (int axis = 0; axis < 3; axis++) {
            const interval& ax = axis_interval(axis);
            // Assuming ray_dir components are accessible via operator[]
            // Assuming interval members 'min' and 'max' are public or have accessors
            const double adinv = 1.0 / ray_dir[axis];

            // Using ax.min and ax.max, which are used in your original code's hit function.
            // This implies 'min' and 'max' are accessible members of the interval class.
            auto t0 = (ax.min - ray_orig[axis]) * adinv;
            auto t1 = (ax.max - ray_orig[axis]) * adinv;

            if (t0 < t1) {
                if (t0 > ray_t.min) ray_t.min = t0;
                if (t1 < ray_t.max) ray_t.max = t1;
            }
            else {
                if (t1 > ray_t.min) ray_t.min = t1;
                if (t0 < ray_t.max) ray_t.max = t0;
            }

            if (ray_t.max <= ray_t.min)
                return false;
        }
        return true;
    }

    int longest_axis() const {
        // Returns the index of the longest axis of the bounding box.
        // Assuming interval has public members 'min' and 'max' to calculate its length.
        // The error E0135 indicates 'interval' has no member 'size'.
        // We calculate the length as max - min.
        double x_len = x.max - x.min;
        double y_len = y.max - y.min;
        double z_len = z.max - z.min;

        if (x_len > y_len)
            return x_len > z_len ? 0 : 2;
        else
            return y_len > z_len ? 1 : 2;
    }

    // Declare static members here
    static const aabb empty;
    static const aabb universe;
};

#endif