#include "geometry.h"

#include <cmath>

bool sphere::hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& record) const {
    vec3 oc = center - r.origin();

    double a = r.direction().length_squared();
    double h = -2.0 * r.direction().dot(oc);
    double c = oc.length_squared() - radius*radius;

    double discriminant = h*h - a*c;

    if (discriminant < 0) {
        return false;
    } else {
        double sqrt_d = std::sqrt(discriminant);

        // find nearest root in acceptable range
        double root = (h - sqrt_d) / a;
        if (root <= ray_tmin || ray_tmax <= root) {
            root = (h + sqrt_d) / a;

            if (root <= ray_tmin || ray_tmax <= root) {
                return false;
            }
        }

        record.t = root;
        record.p = r.at(record.t);
        record.normal = (record.p - center) / radius;

        return true;
    }
}

