#ifndef VEC2_H
#define VEC2_H

#include "../vec3/vec3.h"

typedef struct vector2d { double x, y; } vec2;

vec2 v2add(vec2 v, vec2 w);
vec2 v2sub(vec2 v, vec2 w);
vec2 v2mul(vec2 v, vec2 w);
vec2 v2smul(vec2 v, double s);
vec2 v2div(vec2 v, double d);
vec2 homogenize2d(vec3 h);
double v2len(vec2 v);
vec2 v2normalize(vec2 v);
double v2cross(vec2 v, vec2 w);
double v2dot(vec2 v, vec2 w);

#endif