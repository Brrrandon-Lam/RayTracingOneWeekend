#ifndef HITTABLE_LIST_H_
#define HITTABLE_LIST_H_

#include "generalutilities.h"
#include "hittable.h"
#include <memory>
#include <vector>

class hittable_list : public hittable {
    public:
        // Holds all of our objects
        std::vector<std::shared_ptr<hittable>> objects;

        // Constructor
        hittable_list() {}
        hittable_list(std::shared_ptr<hittable> object) { add(object); }

        void add(std::shared_ptr<hittable> object) { objects.push_back(object); }
        void clear() { objects.clear(); }

        // Override hit
        virtual bool hit(const ray &r, double t_min, double t_max, hit_record& hit_record) const override;
};

bool hittable_list::hit(const ray &r, double t_min, double t_max, hit_record& hit_record) const {
    struct hit_record temp_record;
    bool hit_anything = false;
    auto closest_hit = t_max;

    // Iterate through the list of hits and return whether or not something was hit.
    // Update the hit record accordingly.
    for(const auto &object : objects) {
        if(object->hit(r, t_min, closest_hit, temp_record)) {
            hit_anything = true;
            closest_hit = temp_record.t;
            hit_record = temp_record;
        }
    }
    return hit_anything;
}


#endif