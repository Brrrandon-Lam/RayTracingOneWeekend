#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "generalutilities.h"

struct hit_record;

class material {
    public:
        // Virtual function to determine the outcome of scattering a ray based on the material
        virtual bool scatter(const ray &r_in, const hit_record& rec, color& attenuation, ray &scattered) const = 0;
};

class lambertian : public material {
    public:
        color albedo;

        lambertian(const color &a) : albedo(a) {}
        virtual bool scatter(const ray &r_in, const hit_record& rec, color& attenuation, ray &scattered) const override {
            auto scatter_direction = rec.normal + random_unit_vector();
            // Catch degenerate scatter direction
            if(scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }
            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }
};

class metal : public material {
    public:
        color albedo;
        double fuzziness;

        metal(const color &a, double f) : albedo(a), fuzziness(f < 1 ? f : 1) {}
        virtual bool scatter(const ray &r_in, const hit_record& rec, color& attenuation, ray &scattered) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzziness * random_in_unit_sphere());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
            
        }
};

class dielectric : public material {
    public:
        double index_of_refraction;

        dielectric(double ir) : index_of_refraction(ir) {}
        virtual bool scatter(const ray &r_in, const hit_record& rec, color& attenuation, ray &scattered) const override {
            attenuation = color(1.0, 1.0, 1.0);
            double refraction_ratio = rec.front_face ? (1.0 / index_of_refraction) : index_of_refraction;

            vec3 unit_direction = unit_vector(r_in.direction());

            // Do we refract or reflect?
            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1 - cos_theta * cos_theta);

            bool must_reflect = refraction_ratio * sin_theta > 1.0;
            vec3 direction;
            if(must_reflect || reflectance(cos_theta, refraction_ratio) > random_double()) {
                // Calculate a reflection vector
                direction = reflect(unit_direction, rec.normal);
            }
            else {
                // Calculate refraction vector
                direction = refract(unit_direction, rec.normal, refraction_ratio);
            }

            // Create a ray scattered from the hit position in the direction of the refraction vector.
            scattered = ray(rec.p, direction);

            return true;
        }
    private:
        static double reflectance(double cosine, double ref_idx) {
            // Schlick Approximation for Reflectance
            auto r0 = (1 - ref_idx) / (1 + ref_idx);
            r0 = r0 * r0;
            return r0 + (1 - r0) * pow((1 - cosine), 5);
        }
};

#endif