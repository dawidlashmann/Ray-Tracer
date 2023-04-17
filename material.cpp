#include "material.h"


bool lambertian::scatter(const ray& r_in, const hit_record& rec, color& attent, ray& scattered) const {
    auto scatter_dir = rec.n + random_unit_vec();
    
    if(scatter_dir.near_zero())
        scatter_dir = rec.n;

    scattered = ray(rec.p, scatter_dir, r_in.time());
    attent = albedo;
    return true;
}

bool metal::scatter(const ray& r_in, const hit_record& rec, color& attent, ray& scattered) const {
    vec3 reflected = reflect(unit_vector(r_in.direction()), rec.n);
    scattered = ray(rec.p, reflected + fuzz * random_unit_vec(), r_in.time());
    attent = albedo;
    return (dot(scattered.direction(), rec.n) > 0);
}

bool glass::scatter(const ray& r_in, const hit_record& rec, color& attent, ray& scattered) const {
    attent = color(1.0, 1.0, 1.0);
    float ref_ratio = rec.frnt_face ? 1/ir : ir;
    vec3 temp_n = rec.n;

    vec3 unit_dir = unit_vector(r_in.direction());
    float cos_theta = fmin(dot(-unit_dir, rec.n), 1.0);
    float sin_theta = sqrt(1 - cos_theta * cos_theta);
    vec3 direction;

    if(ref_ratio * sin_theta > 1.0 || reflactance(cos_theta, ref_ratio) > random_db(0.0, 1.0)){
        direction = reflect(unit_dir, rec.n);
    }else{
        direction = refract(unit_dir, rec.n, ref_ratio);
    }

    scattered = ray(rec.p, direction, r_in.time());
    return true;
}

float glass::reflactance(float cosine, float ref_idx){
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 *= r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}