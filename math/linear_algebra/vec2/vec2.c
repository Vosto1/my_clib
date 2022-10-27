#include "vec2.h"

vec2 vec2_create(double x, double y)
{
    vec2 v;
    v.x = x;
    v.y = y;
    return v;
}

vec2 v2add(vec2 v, vec2 w)
{
    vec2 a;
    a.x = v.x + w.x;
    a.y = v.y + w.y;
    return a;
}

vec2 v2sub(vec2 v, vec2 w)
{
    vec2 s;
    s.x = v.x - w.x;
    s.y = v.y - w.y;
    return s;
}

vec2 v2mul(vec2 v, vec2 w)
{
    vec2 m;
    m.x = v.x * w.x;
    m.y = v.y * w.y;
    return m;
}

vec2 v2smul(vec2 v, double s)
{
    vec2 m;
    m.x = v.x * s;
    m.y = v.y * s;
    return m;
}

vec2 v2div(vec2 v, double d)
{
    return v2smul(v, 1.0f / d);
}

vec2 homogenize2d(vec3 h)
{
    vec2 v;
    if (h.z == 0)
    {
        fprintf(stderr, "homogenize2d div0\n");
        v.x = v.y = __DBL_MAX__; // "infinity"
    }
    v.x = h.x / h.z;
    v.y = h.y / h.z;
    return v;
}

double v2len(vec2 v)
{
    sqrt(v.x * v.x + v.y * v.y);
}

vec2 v2normalize(vec2 v)
{
    double l = v2len(v);
    vec2 n;
    n.x = v.x / l;
    n.y = v.y / l;
    return n;
}

// parallellogram area
double v2cross(vec2 v, vec2 w)
{
    return v.x * w.y - v.y * w.x;
}

double v2dot(vec2 v, vec2 w)
{
    return v.x * w.x + v.y * w.y;
}

