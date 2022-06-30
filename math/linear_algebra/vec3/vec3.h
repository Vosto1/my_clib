#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include "../mat4/mat4.h"
#include "../vec4/vec4.h"

typedef struct vector3d { double x, y, z; } vec3;

vec3 vadd(vec3 v, vec3 w);
vec3 vsub(vec3 v, vec3 w);
vec3 cross(vec3 v, vec3 w);
double dot(vec3 v, vec3 w);
double len(vec3 v);
vec3 normalize(vec3 v);
vec3 vsmul(vec3 v, double s);
vec4 vmmul(vec3 v, mat4 m);
vec3 surface_normal(vec3 u, vec3 v, vec3 w);
vec3 vrotate(vec3 v, vec3 axis, double angle);

// debug
vec3 printvec3(char* name, vec3 v);


#endif