#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
    public:
        ray() {}
        ray(const point3 &orig, const vec3 &dire, float time = 0.0) : org(orig), dir(dire), tm(time) {}

        point3 origin() const { return org; }
        vec3 direction() const { return dir; }
        float time() const { return tm; }

        point3 at(float t) const {
            return t*dir + org;
        }
    public:
        point3 org;
        vec3 dir;
        float tm;
};



#endif