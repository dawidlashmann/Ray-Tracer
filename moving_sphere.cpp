#include "moving_sphere.h"


point3 sphere::center(float time) const {
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool sphere::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
    vec3 oc = r.origin() - center(r.time());
    float a = r.direction().length_sq();
    float hb = dot(oc, r.direction());
    float c = oc.length_sq() - radius * radius;
    float delta = hb*hb - a*c;
    if(delta < 0){
        return false;
    }
    auto root = (-hb - sqrt(delta)) / (a);
    if(root < tmin || root > tmax){
        root = (-hb + sqrt(delta)) / (a);
        if(root < tmin || root > tmax){
            return false;
        }
    }
    rec.t = root;
    rec.p = r.at(root);
    vec3 out_n = (rec.p - center(r.time())) / radius;
    rec.set_face_n(r, out_n);
    rec.mat_ptr = mat;

    return true;
}