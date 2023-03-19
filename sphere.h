#ifndef SPHERE_H_
#define SPHERE_H_

#include "generalutilities.h"
#include "hittable.h"

class sphere : public hittable {
    public:
        point3 center;
        double radius;
        std::shared_ptr<material> mat_ptr;

        // Constructors
        sphere() {}
        sphere(point3 sphere_center, double sphere_radius, std::shared_ptr<material> m)
            : center(sphere_center), radius(sphere_radius), mat_ptr(m) {};

        // Override hit
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& hit_record) const override;
};

bool sphere::hit(const ray &r, double t_min, double t_max, hit_record& hit_record) const {
    // Use simplified  form of quadratic equation to return the hit location if the discriminant is positive (hits)
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b * half_b - a * c;
    if(discriminant < 0) { 
        // No hit
        return false; 
    }
    auto sqrt_disc = std::sqrt(discriminant);

    // Find the nearest root within range [t_min, t_max]
    auto root = (-half_b - sqrt_disc) / a;
    if(root < t_min || root > t_max) {
        root = (-half_b + sqrt_disc) / a;
        if(root < t_min || root > t_max) {
            // Not in t-range, false.
            return false;
        }
    }

    // Store results in our out parameter
    hit_record.t = root;
    hit_record.p = r.at(hit_record.t);
    // Calculate the outward normal and set it
    vec3 outward_normal = (hit_record.p - center) / radius;
    hit_record.set_face_normal(r, outward_normal);
    // Update material pointer in the hit record to point to the sphere material.
    hit_record.mat_ptr = mat_ptr;
    // Success
    return true;
}

#endif