#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H
#include "aabb.h"

#include "Hittable.h"

#include <vector>

using std::shared_ptr;

class hittable_list : public hittable {
public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<hittable> object) { 
        
        objects.push_back(object);
        bbox = aabb(bbox, object->bounding_box());

    }

    virtual bool hit(
        const ray& r, interval ray_t, hit_record& rec) const override;
    aabb bounding_box() const override { return bbox; }

public:
    std::vector<shared_ptr<hittable>> objects;


private:
    aabb bbox;
};

bool hittable_list::hit(const ray& r, interval ray_t, hit_record& rec) const {
    bool hit_anything = false;
    auto closest_so_far = ray_t.max;

    for (const auto& object : objects) {
        if (object->hit(r, interval(ray_t.min, closest_so_far), rec)) {
            hit_anything = true;
            closest_so_far = rec.t;
        }
    }

    return hit_anything;
}

#endif