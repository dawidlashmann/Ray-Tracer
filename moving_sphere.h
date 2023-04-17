#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "raytr.h"
#include "hittable.h"

class sphere : public hittable{
public:
    sphere(){}
    sphere(point3 cen0, point3 cen1, float time0_, float time1_, float r, material * m)
                    : center0(cen0), center1(cen1), time0(time0_), time1(time1_), radius(r), mat(m)
    {};

    virtual bool hit(const ray&, float, float, hit_record&) const override;

    point3 center(float) const;



public:
    point3 center0, center1;
    float time0, time1;
    float radius;
    material * mat;
};


#endif