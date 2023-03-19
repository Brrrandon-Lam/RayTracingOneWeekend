#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

class vec3 {
    public:
        // Constructors
        vec3() : e{0, 0, 0 } {}
        vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }

        // Vector Negative
        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }

        // Vector components
        double operator[](int i) const { return e[i]; }
        double& operator[](int i) { return e[i]; }

        // Vector addition
        vec3& operator+=(const vec3 &v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        // Vector multiplication
        vec3& operator*=(const double t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        // Vector division (calls vector multiplication)
        vec3& operator/=(const double t) {
            return *this *= 1/t;
        }

        // Magnitude (Distance)
        double length() const {
            return std::sqrt(length_squared());
        }

        // Square Magnitude (Square Distance)
        double length_squared() const {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

        // Data, double array of size 3
        double e[3];

        inline static vec3 random() {
            return vec3(random_double(), random_double(), random_double());
        }

        inline static vec3 random(double min, double max) {
            return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
        }

        bool near_zero() const {
            // Returns true if hte vector is close to zero in all directions
            const auto s = 1e-8;
            return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < 2);
        }

        // Utilities
};

using point3 = vec3; // 3-component point vector (XYZ)
using color = vec3; // 3-component color vector (RGB)

// Utilities for vec3 class
// Print Vector
inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// Add two vectors
inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

// Subtract two vectors
inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// Multiply two vectors
inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

// Scalar multiplication against vector
inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

// Scalar multiplication against vector (lets us put the scalar value first)
inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

// Vector division
inline vec3 operator/(vec3 v, double t) {
    return (1/t) * v;
}

// Dot Product
inline double dot(const vec3 &u, const vec3 &v) {
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

// Cross Product
inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

// Normalize
inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3::random(-1,1);
        if (p.length_squared() >= 1) {
            continue;
        }
        return p;
    }
}

vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

// Depth of Field - Generate a ray within a disk
vec3 random_unit_in_disk() {
    while(true) {
        // Generate a random point inside of a circle on XY plane w/ radius 1 using rejection method.
        auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if(p.length_squared() >= 1) { 
            continue; 
        }
        return p;
    }
}

// Reflection Vector for Metals
vec3 reflect(const vec3 &v, const vec3 &n) {
    return v - 2 * dot(v, n) * n;
}

// Refraction
vec3 refract(const vec3 &uv, const vec3 &n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perpendicular = etai_over_etat * (uv + cos_theta * n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perpendicular.length_squared())) * n;
    // Calculate the refraction vector by using the dot product to get the parallel and perpendicular components individually.
    return r_out_perpendicular + r_out_parallel;
}

#endif