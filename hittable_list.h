#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

class hittable_list : public hittable{
    public:
        hittable_list(){}
        hittable_list(hittable **l, int n){ list = l; list_size = n;}

        virtual bool hit (const ray &r, float tmin, float tmax, hit_record& rec) const override;

    public:
        hittable **list;
        int list_size;
};



#endif