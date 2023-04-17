#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>
#include <random>

inline double random_db_2(double min, double max){
    static std::uniform_real_distribution<double>distribution(min, max);
    static std::mt19937 generator;
    return distribution(generator);
}

class vec3{
    public:
    vec3() : crd{0, 0, 0} {}
    vec3(float x, float y, float z) : crd{x, y, z} {}

    float x() {return crd[0]; }
    float y() {return crd[1]; }
    float z() {return crd[2]; }

    float operator[] (int i) const {return crd[i]; }
    float &operator[] (int i) {return crd[i]; }
    vec3 operator- () const {return vec3(-crd[0], -crd[1], -crd[2]); }
    
    vec3& operator+= (const vec3& i){
        crd[0] += i.crd[0];
        crd[1] += i.crd[1];
        crd[2] += i.crd[2];
        return *this;
    }
    
    vec3& operator*= (const float t){
        crd[0] *= t;
        crd[1] *= t;
        crd[2] *= t;
        return *this;
    }

    vec3& operator/= (const float t){
        return *this *= 1/t;
    }
    
    float length_sq () const{
        return crd[0]*crd[0] + crd[1]*crd[1] + crd[2]*crd[2];
    }
    
    float length() const{
        return sqrt(length_sq());
    }

    inline static vec3 random(double min, double max){
        return vec3(random_db_2(min, max), random_db_2(min, max), random_db_2(min, max));
    }

    inline bool near_zero() const{
        const auto s = 1e-8;
        return (fabs(crd[0] < s) && fabs(crd[1] < s) && fabs(crd[2] < s));
    }

    public:
    float crd[3];
};

using point3 = vec3;
using color = vec3;

inline std::ostream& operator<<(std::ostream &out, const vec3 &i){
    return out << i.crd[0] << ' ' << i.crd[1] << ' ' << i.crd[2];
}

inline vec3 operator+ (const vec3& a, const vec3& b){
    return vec3(a.crd[0] + b.crd[0], a.crd[1] + b.crd[1], a.crd[2] + b.crd[2]);
}

inline vec3 operator- (const vec3 &a, const vec3 &b){
    return vec3(a.crd[0] - b.crd[0], a.crd[1] - b.crd[1], a.crd[2] - b.crd[2]);
}

inline vec3 operator* (const vec3 &a, const vec3 &b){
    return vec3(a.crd[0] * b.crd[0], a.crd[1] * b.crd[1], a.crd[2] * b.crd[2]);
}

inline vec3 operator* (const vec3 &a, float t){
    return vec3(a.crd[0] * t, a.crd[1] * t, a.crd[2] * t);
}

inline vec3 operator* (float t, const vec3 &a){
    return a * t;
}

inline vec3 operator/ (vec3 a, float t){
    return (1/t) * a;
}

inline float dot(const vec3 &a, const vec3 &b){
    return a.crd[0]*b.crd[0] + a.crd[1]*b.crd[1] + a.crd[2]*b.crd[2];
}

inline vec3 cross(const vec3 &a, const vec3 &b){
    return vec3(a.crd[1] * b.crd[2] - a.crd[2] * b.crd[1],
                a.crd[2] * b.crd[0] - a.crd[0] * b.crd[2],
                a.crd[0] * b.crd[1] - a.crd[1] * b.crd[0]);
}

inline vec3 unit_vector(vec3 a){
    return a / a.length();
}

inline vec3 random_in_unit_sphere(){
    while(true){
        auto p = vec3::random(-1, 1);
        if(p.length_sq() >= 1) continue;
        return p;
    }
}

inline vec3 random_unit_vec(){
    return unit_vector(random_in_unit_sphere());
}

inline vec3 reflect(const vec3&v, const vec3&n){
    return v - 2*dot(v, n) * n;
}

inline vec3 refract(vec3 &uv, const vec3 &n, float factor){
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_perp = factor * (uv + cos_theta * n);
    vec3 r_para = -sqrt(fabs(1.0 - r_perp.length_sq())) * n;
    return r_perp + r_para;
}

inline vec3 random_in_unit_disk(){
    vec3 p;
	do {
		p = 2.0 * vec3(random_db_2(0.0, 1.0), random_db_2(0.0, 1.0), 0) - vec3(1, 1, 0);
	} while (dot(p, p) >= 1.0);
	return p;
}

#endif