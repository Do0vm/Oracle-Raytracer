#include "Interval.h"
#include "Hittable.h"
#include "Color.h"
#include "Sphere_list.h"
#include "Sphere.h"
#include <iostream>
#include "Utilities.h"
#include "Material.h"
#include "Camera.h"
#include <fstream>


using std::make_shared;



color ray_color(const ray& r, const sphere_list& world) {
    hit_record rec;

    interval ray_interval(0.001, infinity); // Small epsilon to avoid shadow acne
    if (world.hit(r, ray_interval, rec)) {
        return 0.5 * (rec.normal + color(1, 1, 1));
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0)
        + t * color(0.5, 0.7, 1.0);
}

int main() {
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    //matt creation
auto default_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));

  

    // World
    sphere_list world;
    world.add(sphere(point3(0, -100.5, -1), 100.0, default_material));
    world.add(sphere(point3(0, 0.0, -1), 0.5, default_material));

    // Camera
    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 20;
    cam.max_depth = 20;

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    cam.render(world);

  

    std::cerr << "\nDone.\n";
    return 0;
}
