#ifndef MATERIAL_H
#define MATERIAL_H

#include "raytr.h"
#include "hittable.h"


class material{
    public:
        virtual bool scatter(const ray&, const hit_record&, color&, ray&) const = 0;
};

class lambertian : public material{
    public:
        lambertian(const color& a) : albedo(a) {}

        virtual bool scatter(const ray&, const hit_record&, color&, ray&) const override;


    public:
        color albedo;
};

class metal : public material{
    public:
        metal(const color &a, float f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(const ray&, const hit_record&, color&, ray&) const override;

    public:
        color albedo;
        float fuzz;
};

class glass : public material{
    public:
        glass(float irr) : ir(irr) {}

        virtual bool scatter(const ray&, const hit_record&, color&, ray&) const override;

        static float reflactance(float cosine, float ref_idx);

    public:
        double ir;
};


#endif