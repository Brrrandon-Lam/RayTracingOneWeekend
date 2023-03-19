#ifndef GENERALUTILITIES_H_
#define GENERALUTILITIES_H_

#include <cmath>
#include <limits>
#include <memory>
#include "ray.h"
#include "vec3.h"
// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Convert degrees to radians
inline double to_radians(double degrees) {
    return degrees * pi / 180.0;
}


#endif