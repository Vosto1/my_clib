#include "vec4.h"

vec4 vec4_create(double x, double y, double z, double w)
{
    vec4 v;
    v.x = x;
    v.y = y;
    v.z = z;
    v.w = w;
    return v;
}

vec3 homogenize(vec4 h)
{
    vec3 v;
    if (h.w == 0)
    {
        fprintf(stderr, "homogenize div0\n");
        v.x = v.y = v.z = __DBL_MAX__; // "infinity"
    }
    v.x = h.x / h.w;
    v.y = h.y / h.w;
    v.z = h.z / h.w;
    return v;
}