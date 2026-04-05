#include "renderer.h"

rendering::rendering(int width, int height, double focal_length) : width(width), height(height), focal_length(focal_length) {
    viewport_height = 2.0;
    viewport_width = (width / height) * viewport_height;

    viewport_u = vec3(viewport_width, 0, 0);
    viewport_v = vec3(0, -viewport_height, 0);

    pixel_delta_u = viewport_u / width;
    pixel_delta_v = viewport_v / height;

    viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
}

void rendering::add_object(std::unique_ptr<screen_object> object) {
    objects.push_back(std::move(object));
}

color rendering::ray_color(const ray& r) const {
    // create a gradient
    vec3 unit_direction = r.direction().unit_vector();
    double a =  0.5 * (unit_direction[1] + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}

color rendering::pixel_at(int x, int y) const {
    point3 pixel_center = pixel00_loc + x*pixel_delta_u + y*pixel_delta_v;
    vec3 ray_direction = pixel_center - camera_center;
    ray r(camera_center, ray_direction);

    return ray_color(r);
}

