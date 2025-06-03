#ifndef BVH_H
#define BVH_H

#include "aabb.h"        // For aabb
#include "hittable.h"    // For hittable, shared_ptr, make_shared (likely via common_headers or hittable itself)
#include "hittable_list.h" // For hittable_list and its members

#include <algorithm>     // For std::sort
#include <vector>        // For std::vector
#include <memory>        // For std::shared_ptr, std::make_shared
#include "Utilities.h"

// Forward declaration for random_int if not included above
// int random_int(int min, int max);

class bvh_node : public hittable {
public:
    // Constructor that takes the initial list of objects
    bvh_node(hittable_list list)

    {
   
        std::vector<std::shared_ptr<hittable>> local_objects = list.objects; // Make a mutable copy
        build_bvh(local_objects, 0, local_objects.size());
    }

    

    // Recursive helper function or a constructor that takes objects by reference
    bvh_node(std::vector<std::shared_ptr<hittable>>& local_objects, size_t start, size_t end) {
        // Build the bounding box of the span of source objects.
        // This part was originally in the second constructor and is correctly placed.
        bbox = aabb::empty;
        for (size_t object_index = start; object_index < end; object_index++) {
            bbox = aabb(bbox, local_objects[object_index]->bounding_box());
        }

        // The splitting logic, previously in the first constructor, belongs here,
        // operating on local_objects, start, and end.
        int axis = bbox.longest_axis(); // Or random_int(0,2) as in the original 

        auto comparator = (axis == 0) ? box_x_compare
            : (axis == 1) ? box_y_compare
            : box_z_compare;

        size_t object_span = end - start;

        if (object_span == 1) {
            left = right = local_objects[start];
        }
        else if (object_span == 2) {
            // Sort the two elements if the comparator matters for order
            if (comparator(local_objects[start + 1], local_objects[start])) {
                std::swap(local_objects[start], local_objects[start + 1]);
            }
            left = local_objects[start];
            right = local_objects[start + 1];
        }
        else {
            // Correctly sort the sub-range of the vector
            std::sort(local_objects.begin() + start, local_objects.begin() + end, comparator);

            auto mid = start + object_span / 2;
            left = std::make_shared<bvh_node>(local_objects, start, mid);
            right = std::make_shared<bvh_node>(local_objects, mid, end);
        }
    }


    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        if (!bbox.hit(r, ray_t))
            return false;

        // Make sure left and right are not null
        bool hit_left = false;
        if (left) { // Add null check if `left` can be null (e.g. empty leaf)
            hit_left = left->hit(r, ray_t, rec);
        }

        bool hit_right = false;
        if (right) { // Add null check
            // If left was hit, ray_t.max might have been updated via rec.t
            interval right_ray_t = hit_left ? interval(ray_t.min, rec.t) : ray_t;
            hit_right = right->hit(r, right_ray_t, rec);
        }

        return hit_left || hit_right;
    }

    aabb bounding_box() const override { return bbox; }

private:
    // Helper to encapsulate the recursive construction if preferred
    // This is an alternative to having the second constructor public or directly called.
    void build_bvh(std::vector<std::shared_ptr<hittable>>& build_objects, size_t start, size_t end) {
        // Calculate overall bounding box for the current node's primitives
        bbox = aabb::empty;
        for (size_t i = start; i < end; ++i) {
            bbox = aabb(bbox, build_objects[i]->bounding_box());
        }

        // int axis = random_int(0, 2); // As per original first constructor
        int axis = bbox.longest_axis(); // More common for better splits

        auto comparator = (axis == 0) ? box_x_compare
            : (axis == 1) ? box_y_compare
            : box_z_compare;

        size_t object_span = end - start;

        if (object_span == 1) {
            left = right = build_objects[start];
        }
        else if (object_span == 2) {
            if (comparator(build_objects[start + 1], build_objects[start])) { // Ensure order if comparator matters
                std::swap(build_objects[start], build_objects[start + 1]);
            }
            left = build_objects[start];
            right = build_objects[start + 1];
        }
        else {
            std::sort(build_objects.begin() + start, build_objects.begin() + end, comparator);
            auto mid = start + object_span / 2;
            left = std::make_shared<bvh_node>(build_objects, start, mid); // Pass the vector by reference
            right = std::make_shared<bvh_node>(build_objects, mid, end);  // Pass the vector by reference
        }
    }

  

    std::shared_ptr<hittable> left;
    std::shared_ptr<hittable> right;
    aabb bbox;

    // Static comparison functions remain the same
    static bool box_compare(
        const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b, int axis_index
    ) {
        // Ensure bounding_box() is const and axis_interval() is const
        aabb box_a = a->bounding_box();
        aabb box_b = b->bounding_box();
        auto a_axis_interval = box_a.axis_interval(axis_index);
        auto b_axis_interval = box_b.axis_interval(axis_index);
        return a_axis_interval.min < b_axis_interval.min;
    }

    static bool box_x_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
        return box_compare(a, b, 0);
    }

    static bool box_y_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
        return box_compare(a, b, 1);
    }

    static bool box_z_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
        return box_compare(a, b, 2);
    }

    // Assume random_int is defined elsewhere, e.g., rtweekend.h
    // static int random_int(int min, int max) { ... }
};

#endif