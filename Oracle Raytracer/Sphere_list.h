#ifndef SPHERE_LIST_H
#define SPHERE_LIST_H

#include "Hittable.h"
#include "Sphere.h"
#include <vector>
#include <memory>

using std::shared_ptr;
using std::make_shared;

class sphere_list : public hittable {
public:
    sphere_list() = default;

    // add a sphere by value
    void add(const sphere& s) {
        objects.push_back(make_shared<sphere>(s));
    }

    // clear all spheres
    void clear() {
        objects.clear();
    }

    // test ray against all spheres in the list
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        bool hit_anything = false;
        double closest_so_far = ray_t.max;
        hit_record temp_rec;

        for (const auto& obj : objects) {
            // restrict t to [ray_t.min, closest_so_far]
            interval t_interval{ ray_t.min, closest_so_far };
            if (obj->hit(r, t_interval, temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }

private:
    std::vector<shared_ptr<sphere>> objects;
};

#endif // SPHERE_LIST_H
