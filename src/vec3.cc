#include "vec3.h"

#include <cmath>
#include <limits>

#include "util.h"

vec3 vec3::operator-() const {
    return vec3{-e[0], -e[1], -e[2]};
}

double vec3::operator[](int i) const {
    return e[i];
}

double& vec3::operator[](int i) {
    return e[i];
}

vec3& vec3::operator+=(const vec3& v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
}

vec3& vec3::operator*=(const vec3& v) {
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];
    return *this;
}

vec3& vec3::operator*=(double t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

vec3& vec3::operator/=(double t) {
    return *this *= 1/t;
}

vec3 vec3::operator+(const vec3& v) const {
    return vec3{e[0] + v.e[0], e[1] + v.e[1], e[2] + v.e[2]};
}

vec3 vec3::operator-(const vec3& v) const {
    return vec3{e[0] - v.e[0], e[1] - v.e[1], e[2] - v.e[2]};
}

vec3 vec3::operator*(const vec3& v) const {
    return vec3{e[0] * v.e[0], e[1] * v.e[1], e[2] * v.e[2]};
}

vec3 vec3::operator/(double t) const {
    return (1/t) * (*this);
}

double vec3::dot(const vec3& v) const {
    return e[0] * v.e[0]
         + e[1] * v.e[1]
         + e[2] * v.e[2];
}

vec3 vec3::cross(const vec3& v) const {
    return vec3(e[1] * v.e[2] - e[2] * v.e[1],
            e[2] * v.e[0] - e[0] * v.e[2],
            e[0] * v.e[1] - e[1] * v.e[0]);
}

vec3 vec3::unit_vector() const {
    return *this / length();
}

double vec3::length() const {
    return std::sqrt(length_squared());
}

double vec3::length_squared() const {
    return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
}


std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

vec3 operator*(double t, const vec3& v) {
    return vec3(t*v[0], t*v[1], t*v[2]);
}

bool vec3::near_zero() const {
    static double epsilon = 1e-8;
    return std::abs(e[0]) < epsilon && std::abs(e[1]) < epsilon && std::abs(e[2]) < epsilon;
}

vec3 vec3::reflected(const vec3& normal) const {
    return (*this) - 2 * dot(normal) * normal;
}

vec3 vec3::refracted(const vec3& normal, double eta_i_over_eta_t) const {
    double cos_theta = std::min((-(*this)).dot(normal), 1.0);
    vec3 r_out_perp = eta_i_over_eta_t * ((*this) + cos_theta*normal);
    vec3 r_out_parallel = -std::sqrt(std::abs(1.0 - r_out_perp.length_squared())) * normal;

    return r_out_perp + r_out_parallel;
}

vec3 vec3::random() {
    return vec3(random_double(), random_double(), random_double());
}

vec3 vec3::random(double min, double max) {
    return (max - min) * vec3::random() + vec3(min, min, min);
}

vec3 vec3::random_unit() {
    while (true) {
        vec3 p = vec3::random(-1,1);
        double len_sq = p.length_squared();
        if (len_sq > std::numeric_limits<double>::epsilon()
            && len_sq <= 1) {
            return p.unit_vector();
        }
    }
}

