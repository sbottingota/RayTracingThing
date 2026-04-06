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

vec3 vec3::random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = vec3::random_unit();

    // check if vector is in the same hemisphere as normal
    if (on_unit_sphere.dot(normal) > 0.0)  {
        return on_unit_sphere;
    } else {
        return -on_unit_sphere;
    }
}

