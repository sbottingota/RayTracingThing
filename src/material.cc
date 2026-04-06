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
    reflected = reflected.unit_vector() + fuzz * vec3::random_unit();
    scattered = ray(record.p, reflected);
    attenuation = albedo;
    return scattered.direction().dot(record.normal) > 0;
}

bool dielectric::scatter(const ray& ray_in, const hit_record& record, color& attenuation, ray& scattered) const {
    attenuation = color(1.0, 1.0, 1.0);
    double ri = record.front_face ? (1.0 / refractive_index) : refractive_index;

    vec3 unit_direction = ray_in.direction().unit_vector();
    vec3 refracted = unit_direction.refracted(record.normal, ri);

    scattered = ray(record.p, refracted);
    return true;
}

