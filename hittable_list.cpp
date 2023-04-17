#include "hittable_list.h"


bool hittable_list::hit(const ray &r, float tmin, float tmax, hit_record& rec) const {
    hit_record temp_rec;
    bool hot = false;
    auto closest = tmax;

    for(int i = 0; i < list_size; i++){
        if(list[i] -> hit(r, tmin, closest, temp_rec)){
            hot = true;
            closest = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hot;
}