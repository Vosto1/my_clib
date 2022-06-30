#include "vec3.h"

vec3 vadd(vec3 v, vec3 w)
{
    vec3 a;
    a.x = v.x + w.x;
    a.y = v.y + w.y;
    a.z = v.z + w.z;
    return a;
}

vec3 vsub(vec3 v, vec3 w)
{
    vec3 s;
    s.x = v.x - w.x;
    s.y = v.y - w.y;
    s.z = v.z - w.z;
    return s;
}

vec3 cross(vec3 v, vec3 w)
{
    vec3 c;
    c.x = v.y * w.z - v.z * w.y;
    c.y = v.z * w.x - v.x * w.z;
    c.z = v.x * w.y - v.y * w.x;
    return c;
}

double dot(vec3 v, vec3 w)
{
    return v.x * w.x + v.y * w.y + v.z * w.z;
}

double len(vec3 v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3 normalize(vec3 v)
{
    double l = len(v);
    vec3 n;
    n.x = v.x / l;
    n.y = v.y / l;
    n.z = v.z / l;
    return n;
}

vec3 vsmul(vec3 v, double s)
{
    v.x *= s;
    v.y *= s;
    v.z *= s;
    return v;
}

vec4 vmmul(vec3 v, mat4 m)
{
    vec4 h;
    h.x = v.x * m.i[0] + v.y * m.i[4] + v.z * m.i[8] + m.i[12];
    h.y = v.x * m.i[1] + v.y * m.i[5] + v.z * m.i[9] + m.i[13];
    h.z = v.x * m.i[2] + v.y * m.i[6] + v.z * m.i[10] + m.i[14];
    h.w = v.x * m.i[3] + v.y * m.i[7] + v.z * m.i[11] + m.i[15];
    return h;
}

vec3 surface_normal(vec3 u, vec3 v, vec3 w)
{
    return normalize(cross(vsub(u, v), vsub(u, w)));
}

vec3 vrotate(vec3 v, vec3 axis, double angle)
{
    // project vector to rotate (v) to get coincident component
    vec3 coinccomp = vsmul(axis, dot(v, axis) / dot(axis, axis));
    // subtract the concident component from vector to rotate (v) to get the right angled component
    vec3 racomp = vsub(v, coinccomp);
    // cross prod of right angle component and axis to get a vector orthogonal to both
    vec3 orthog = cross(axis, racomp);
    // calculate rotation scalars for the right angled vectors
    double x1 = cos((angle * M_PI) / 180.0f) / len(racomp);
    double x2 = sin((angle * M_PI) / 180.0f) / len(orthog);
    // get the rotated right angled vector
    vec3 rarotated = vsmul(vadd(vsmul(racomp, x1), vsmul(orthog, x2)), len(racomp));
    // assemble the vector components
    return vadd(coinccomp, rarotated);
}

vec3 printvec3(char* name, vec3 v)
{
    printf("%s: %6.5lf %6.5lf %6.5lf\n", name, v.x, v.y, v.z);
}

