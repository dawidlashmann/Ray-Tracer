#include "camera.h"

camera::camera(point3 look_from, point3 look_at, vec3 vup, float vfov, float ratio, float aperature, float focus, float time_0 = 0, float time_1 = 0){
    auto theta = deg_to_rad(vfov);
    auto h = tan(theta / 2.0);
    auto vp_h = 2.0 * h;
    auto vp_w = ratio * vp_h;

    w = unit_vector(look_from - look_at);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    origin = look_from;
    hrz = focus * vp_w * u;
    vrt = focus * vp_h * v;
    lower_left_corner = origin - hrz / 2.0 - vrt / 2.0 - focus * w;

    lens_r = aperature / 2;
    time0 = time_0;
    time1 = time_1;
}