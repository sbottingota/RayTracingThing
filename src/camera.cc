#include "camera.h"

#include <cmath>
#include <cstdlib>

#include "util.h"
#include "material.h"

constexpr double inf = std::numeric_limits<double>::infinity();

camera::camera(int width, int height, double focal_length, std::shared_ptr<screen_object> object)
    : width(width), height(height), focal_length(focal_length), object(object) {

    viewport_height = 2.0;
    viewport_width = width * viewport_height / height;

    viewport_u = vec3(viewport_width, 0, 0);
    viewport_v = vec3(0, -viewport_height, 0);

    pixel_delta_u = viewport_u / width;
    pixel_delta_v = viewport_v / height;

    viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
}

void camera::set_object(std::shared_ptr<screen_object> object) {
    this->object = object;
}

color camera::ray_color(const ray& r, int depth) const {
    if (depth <= 0) {
        return color(0, 0, 0);
    }

    hit_record record;
    if (object->hits(r, interval(0.01, inf), record)) {
        ray scattered;
        color attenuation;

        if (record.mat->scatter(r, record, attenuation, scattered)) {
            return attenuation * ray_color(scattered, depth - 1);
        } else {
            return color(0, 0, 0);
        }
    }

    // create a gradient
    vec3 unit_direction = r.direction().unit_vector();
    double a =  0.5 * (unit_direction[1] + 1.0);
    return (1.0-a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

color camera::pixel_at(int x, int y) const {
    point3 pixel_center = pixel00_loc + x*pixel_delta_u + y*pixel_delta_v;
    vec3 ray_direction = pixel_center - camera_center;
    ray r(camera_center, ray_direction);

    return ray_color(r, max_depth);
}

color camera::sampled_pixel_at(int x, int y) const {
    color pixel_color(0, 0, 0);

    for (int i = 0; i < samples_per_pixel; ++i) {
        pixel_color += pixel_at(x + random_double()-0.5, y + random_double()-0.5);
    }

    pixel_color /= samples_per_pixel;
    return pixel_color;
}

