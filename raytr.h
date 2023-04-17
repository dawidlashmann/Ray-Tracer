#ifndef RAYTR_H
#define RAYTR_H

#include <memory>
#include <cmath>
#include <limits>
#include <random>



using std::shared_ptr;
using std::make_shared;
using std::sqrt;


const double pi = 3.1415926535897932385;
const double inf = std::numeric_limits<double>::infinity();

inline double deg_to_rad(float deg){
    return deg * pi / 180.0;
}
inline double random_db(double min, double max){
    static std::uniform_real_distribution<double>distribution(min, max);
    static std::mt19937 generator;
    return distribution(generator);
}
inline double clamp(double x, double min, double max){
    if(x > max) return max;
    if(x < min) return min;
    return x;
}

#include "vec3.h"
#include "ray.h"


#endif