#include "Color.h"    // or whichever headers you need
#include "Vector.h"
#include <iostream>


// turn a direction into a sky‐gradient color
color getColor(const vec3& direction) {
    vec3 unit_dir = unit_vector(direction);
    auto t = 0.5 * (unit_dir.y() + 1.0);
    // lerp( white, light blue, t )
    return (1.0 - t) * color(1.0, 1.0, 1.0)
        + t * color(0.5, 0.7, 1.0);
}

int main() {
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 16;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera
    const auto viewport_height = 2.0;
    const auto viewport_width = aspect_ratio * viewport_height;
    const auto focal_length = 1.0;

    const auto origin = point3(0, 0, 0);
    const auto horizontal = vec3(viewport_width, 0, 0);
    const auto vertical = vec3(0, viewport_height, 0);
    const auto lower_left_corner =
        origin
        - horizontal / 2
        - vertical / 2
        - vec3(0, 0, focal_length);

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "Scanlines remaining: " << j << '\n';
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);
            vec3 vec = lower_left_corner
                + u * horizontal
                + v * vertical
                - origin;

            // this creates a 'color' variable called 'pixel_color'
            color pixel_color = getColor(vec);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
    return 0;
}
