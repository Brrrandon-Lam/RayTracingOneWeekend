#ifndef HITTABLE_H_
#define HITTABLE_H_

#include "generalutilities.h"

// Forward Declaration
class material;

struct hit_record {
    point3 p;
    vec3 normal;
    std::shared_ptr<material> mat_ptr;
    double t;
    bool front_face;

    // Set the normal based on front face being true or false
    inline void set_face_normal(const ray &r, const vec3 &outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& record) const = 0;
};

#endif