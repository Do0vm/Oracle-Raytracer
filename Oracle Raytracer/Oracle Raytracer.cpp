#include "Interval.h"
#include "Hittable.h"
#include "Hittable_list.h"
#include "Color.h"
#include "Sphere_list.h"
#include "Sphere.h"
#include <iostream>
#include "Utilities.h"
#include "Material.h"
#include "Camera.h"
#include <fstream>
#include "bvh.h"

using std::make_shared;

int main() {
    hittable_list world;
    world = hittable_list(make_shared<bvh_node>(world));

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    // Scale factor to make the humanoid larger
    double scale = 2.5;

    // Base center of the humanoid, raised to be more central in the view
    point3 base_center(0, 1.5, 0);
    auto humanoid_material = make_shared<metal>(color(0.8, 0.6, 0.4), 0.1);

    // Head (scaled radius)
    world.add(make_shared<sphere>(base_center + vec3(0, 0.6, 0) * scale, 0.25 * scale, humanoid_material));

    // Torso (combined into one larger sphere for simplicity)
    world.add(make_shared<sphere>(base_center, 0.4 * scale, humanoid_material));

    // --- MODIFICATIONS START ---

    // --- Arms ---
    // Using a three-sphere structure for more connected limbs.
    // Base z-offset for arms to bring them forward toward the camera.
    double arm_z_offset_base = 0.1;

    // Left arm (Shoulder -> Bicep -> Forearm)
    world.add(make_shared<sphere>(base_center + (vec3(-0.30, 0.20, arm_z_offset_base) * scale), 0.12 * scale, humanoid_material)); // Shoulder
    world.add(make_shared<sphere>(base_center + (vec3(-0.32, 0.04, arm_z_offset_base) * scale), 0.11 * scale, humanoid_material)); // Bicep
    world.add(make_shared<sphere>(base_center + (vec3(-0.35, -0.15, arm_z_offset_base) * scale), 0.10 * scale, humanoid_material)); // Forearm

    // Right arm (Shoulder -> Bicep -> Forearm)
    world.add(make_shared<sphere>(base_center + (vec3(0.30, 0.20, arm_z_offset_base) * scale), 0.12 * scale, humanoid_material));  // Shoulder
    world.add(make_shared<sphere>(base_center + (vec3(0.32, 0.04, arm_z_offset_base) * scale), 0.11 * scale, humanoid_material));  // Bicep
    world.add(make_shared<sphere>(base_center + (vec3(0.35, -0.15, arm_z_offset_base) * scale), 0.10 * scale, humanoid_material));  // Forearm

    // --- Legs ---
    // Added a z-offset to bring legs forward
    double leg_z_offset_base = 0.05;

    // Left leg
    world.add(make_shared<sphere>(base_center + (vec3(-0.15, -0.5, leg_z_offset_base) * scale), 0.15 * scale, humanoid_material)); // Thigh
    world.add(make_shared<sphere>(base_center + (vec3(-0.15, -0.9, leg_z_offset_base) * scale), 0.14 * scale, humanoid_material)); // Shin

    // Right leg
    world.add(make_shared<sphere>(base_center + (vec3(0.15, -0.5, leg_z_offset_base) * scale), 0.15 * scale, humanoid_material));  // Thigh
    world.add(make_shared<sphere>(base_center + (vec3(0.15, -0.9, leg_z_offset_base) * scale), 0.14 * scale, humanoid_material)); // Shin
  /*  auto center2 = center + vec3(0, random_double(0, .5), 0);
    world.add(make_shared<sphere>(center, center2, 0.2, sphere_material)); */// for move spheres
    // --- MODIFICATIONS END ---

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800; // Using the image width from your latest code
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 60; // Widened field of view to see the whole humanoid
    cam.lookfrom = point3(0, 1.5, 4); // Moved camera closer and centered
    cam.lookat = base_center; // Look directly at the humanoid's torso
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.1;
    cam.focus_dist = (cam.lookfrom - cam.lookat).length(); // Set focus distance to the humanoid

    cam.render(world);
}