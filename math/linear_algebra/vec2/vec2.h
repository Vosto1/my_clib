#ifndef VEC2_H
#define VEC2_H

typedef struct vector2d { double x, y; } vec2;

vec2 v2add(vec2 v, vec2 w);
vec2 v2sub(vec2 v, vec2 w);
vec2 v2mul(vec2 v, vec2 w);
vec2 v2smul(vec2 v, double s);
vec2 v2div(vec2 v, double d);
double v2len(vec2 v);
vec2 v2normalize(vec2 v);
vec2 v2cross(vec2 v, vec2 w);
double v2dot(vec2 v);

#endif