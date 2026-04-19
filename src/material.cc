#include "material.h"

#include <cmath>

bool lambertian::scatter(const ray& ray_in, const hit_record& record, color& attenuation, ray& scattered) const {
    vec3 scatter_direction = record.normal + vec3::random_unit();
    if (scatter_direction.near_zero()) { // catch degenerate scatter direction
        scatter_direction = record.normal;
    }

    scattered = ray(record.p, scatter_direction, ray_in.time());
    attenuation = albedo;
    return true;
}

bool metal::scatter(const ray& ray_in, const hit_record& record, color& attenuation, ray& scattered) const {
    vec3 reflected = ray_in.direction().reflected(record.normal);
    reflected = reflected.unit_vector() + fuzz * vec3::random_unit();

    scattered = ray(record.p, reflected, ray_in.time());
    attenuation = albedo;

    return scattered.direction().dot(record.normal) > 0;
}

bool dielectric::scatter(const ray& ray_in, const hit_record& record, color& attenuation, ray& scattered) const {
    attenuation = color(1.0, 1.0, 1.0);
    double ri = record.front_face ? (1.0 / refractive_index) : refractive_index;

    vec3 unit_direction = ray_in.direction().unit_vector();
    double cos_theta = std::min((-unit_direction).dot(record.normal), 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

    bool can_refract = ri * sin_theta <= 1.0;

    vec3 direction;
    if (can_refract) {
        direction = unit_direction.refracted(record.normal, ri);
    } else {
        direction = unit_direction.reflected(record.normal);
    }

    scattered = ray(record.p, direction, ray_in.time());
    return true;
}

double reflectance(double cosine, double refractive_index) {
    // using schlick's approximation
    double r0 = (1 - refractive_index) / (1 + refractive_index);
    r0 *= r0;
    return r0 + (1 - r0) * std::pow(1 - cosine, 5);    
}

