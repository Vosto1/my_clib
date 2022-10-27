#ifndef VEC4_H
#define VEC4_H

#include <limits.h>
#include <stdio.h>
#include "../vec3/vec3.h"

typedef struct vector4d { double x, y, z, w; } vec4;

vec4 vec4_create(double x, double y, double z, double w);

vec3 homogenize(vec4 h);

#endif