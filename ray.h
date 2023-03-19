#ifndef RAY_H_
#define RAY_H_

#include "vec3.h"

class ray {
    public:
        point3 orig;
        vec3 dir;

        // Constructors
        ray() {}
        ray(const point3& origin, const vec3& direction)
            : orig(origin), dir(direction)
        {}

        // Accessors
        point3 origin() const  { return orig; }
        vec3 direction() const { return dir; }

        // Members

        vec3 at(double t) const {
            // P(t) = A + tb
            return orig + (t * dir);
        }

        
};

#endif