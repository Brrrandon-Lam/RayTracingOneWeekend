#ifndef CAMERA_H
#define CAMERA_H

#include "generalutilities.h"

class camera {
    public:
        // Construct camera with settings for aspect ratio and vertical FOV
        camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov, double aspect_ratio, double aperture, double focus_dist) {
            // Convert vertical field of view to radians.
            auto theta = to_radians(vfov);
            // Calculate height
            auto h = tan(theta/2);
            auto viewport_height = 2.0 * h;
            auto viewport_width = aspect_ratio * viewport_height;
            // auto focal_length = 1.0;

            // Define the Z-axis of our space
            w = unit_vector(lookfrom - lookat);
            // Define the X-axis of our space
            u = unit_vector(cross(vup, w));
            // Define the Y-Axis of our space
            v = cross(w, u);

            origin = lookfrom;
            horizontal = focus_dist * viewport_width * u;
            vertical = focus_dist * viewport_width * v;
            // Instead of focal length, use the Z vector.
            lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist * w;

            lens_radius = aperture / 2;
        }

        ray get_ray(double s, double t) const {
            // Get a random offset location inside of a disk
            vec3 radial_defocus = lens_radius * random_unit_in_disk();
            // Set the offset vector
            vec3 offset = u * radial_defocus.x() + v * radial_defocus.y();
            // Add offset to the origin, subtract offset from destination
            return ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset);
        }

    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;
        double lens_radius;
};
#endif