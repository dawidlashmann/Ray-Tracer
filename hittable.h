#ifndef HITTABLE_H
#define HITTABLE_H

#include "raytr.h"

class material;

struct hit_record {
    point3 p;
    vec3 n;
    material* mat_ptr;
    float t;
    bool frnt_face;

    inline void set_face_n(const ray& r, const vec3& out_nrml){
        frnt_face = dot(r.direction(), out_nrml) < 0;
        n = frnt_face ? out_nrml : ((-1) * out_nrml);
    }
};

class hittable{
    public:
        virtual bool hit(const ray &r, float tmin, float tmax, hit_record& rec) const = 0;
};


#endif