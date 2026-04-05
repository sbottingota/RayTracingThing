#include "renderer.h"

#include <cmath>

#include "interval.h"

constexpr double inf = std::numeric_limits<double>::infinity();

rendering::rendering(int width, int height, double focal_length, std::shared_ptr<screen_object> object)
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

void rendering::set_object(std::shared_ptr<screen_object> object) {
    this->object = object;
}

/*
// temporary solution; TODO: encapsulate this into a sphere class
double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = center - r.origin();
    auto a = r.direction().dot(r.direction());
    auto b = -2.0 * r.direction().dot(oc);
    auto c = oc.dot(oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-b - std::sqrt(discriminant) ) / (2.0*a);
    }
}
*/

color rendering::ray_color(const ray& r) const {
    hit_record record;
    if (object->hits(r, interval(0, inf), record)) {
        vec3 N = (r.at(record.t) - vec3(0, 0, -1)).unit_vector();
        return 0.5*color(N[0]+1, N[1]+1, N[2]+1);
    }

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

