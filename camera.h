#ifndef CAMERA_H
#define CAMERA_H

#include "raytr.h"


class camera{
    public:
        camera(point3, point3, vec3, float, float, float, float, float, float);

        ray get_ray(float s, float t){
            vec3 rd = lens_r * random_in_unit_disk();
            vec3 offset = u * rd.x() + v * rd.y();

            return ray(origin + offset, unit_vector(lower_left_corner + s * hrz + t * vrt - origin - offset), random_db(time0, time1));
        }

    public:
        point3 origin;
        point3 lower_left_corner;
        vec3 hrz;
        vec3 vrt;
        vec3 u, v, w;
        float lens_r;
        float time0, time1;
};


#endif