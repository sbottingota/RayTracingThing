#include <iostream>

#include "vec3.h"
#include "color.h"
#include "ray.h"

#define WIDTH 1280
#define HEIGHT 720

#define FOCAL_LENGTH 1.0

color ray_color(const ray& r) {
    // create a gradient
    vec3 unit_direction = r.direction().unit_vector();
    double a =  0.5 * (unit_direction[1] + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}

int main() {
    double viewport_height = 2.0;
    double viewport_width = (WIDTH / HEIGHT) * viewport_height;
    point3 camera_center = point3(0, 0, 0);

    vec3 viewport_u = vec3(viewport_width, 0, 0);
    vec3 viewport_v = vec3(0, -viewport_height, 0);

    vec3 pixel_delta_u = viewport_u / WIDTH;
    vec3 pixel_delta_v = viewport_v / HEIGHT;

    point3 viewport_upper_left = camera_center - vec3(0, 0, FOCAL_LENGTH) - viewport_u / 2 - viewport_v / 2;
    point3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    std::cout << "P3\n" << WIDTH << ' ' << HEIGHT << "\n255\n";

    for (int j = 0; j < HEIGHT; ++j) {
        std::clog << "\rScanlines: " << j << '/' << WIDTH - 1 << std::flush;

        for (int i = 0; i < WIDTH; i++) {
            point3 pixel_center = pixel00_loc + i*pixel_delta_u + j*pixel_delta_v;
            vec3 ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rRendering complete.                 \n";
}

