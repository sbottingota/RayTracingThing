#include "geometry.h"

void object_group::add(std::shared_ptr<screen_object> object) {
    objects.push_back(object);
}

bool object_group::hits(const ray& r, double ray_tmin, double ray_tmax, hit_record& record) const {
    hit_record temp_record;
    bool has_hit = false;

    double closest_so_far = ray_tmax;

    for (const std::shared_ptr<screen_object>& object : objects) {
        if (object->hits(r, ray_tmin, closest_so_far, temp_record)) {
            has_hit = true;
            closest_so_far = temp_record.t;
            record = temp_record;
        }
    }

    return has_hit;
}

