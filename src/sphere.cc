#include "geometry.h"

#include <cmath>

bool sphere::hits(const ray& r, interval ray_t, hit_record& record) const {
    vec3 oc = center - r.origin();
    auto a = r.direction().length_squared();
    auto h = r.direction().dot(oc);
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = h*h - a*c;
    if (discriminant < 0) {
        return false;
    }

    auto sqrt_d = std::sqrt(discriminant);

    // find the nearest root that lies in the acceptable range
    auto root = (h - sqrt_d) / a;
    if (!ray_t.surrounds(root)) {
        root = (h + sqrt_d) / a;
        if (!ray_t.surrounds(root)) {
            return false;
        }
    }

    record.t = root;
    record.p = r.at(record.t);
    vec3 outward_normal = (record.p - center) / radius;
    record.set_face_normal(r, outward_normal);

    return true;
}

