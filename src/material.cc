#include "material.h"

bool lambertian::scatter(const ray& ray_in, const hit_record& record, color& attenuation, ray& scattered) const {
    vec3 scatter_direction = record.normal + vec3::random_unit();
    if (scatter_direction.near_zero()) { // catch degenerate scatter direction
        scatter_direction = record.normal;
    }

    scattered = ray(record.p, scatter_direction);
    attenuation = albedo;
    return true;
}

bool metal::scatter(const ray& ray_in, const hit_record& record, color& attenuation, ray& scattered) const {
    vec3 reflected = ray_in.direction().reflected(record.normal);
    scattered = ray(record.p, reflected);
    attenuation = albedo;
    return true;
}

