#ifndef LINALG_H
#define LINALG_H

#include <stdio.h>
#include <limits.h>
#include "math2.h"

typedef struct vector2d { double x, y; } vec2;
typedef struct vector3d { double x, y, z; } vec3;
typedef struct vector4d { double x, y, z, w; } vec4;
typedef struct matrix2x2d { double i[4]; } mat2;
typedef struct matrix3x3d { double i[9]; } mat3;
typedef struct matrix4x4d { double i[16]; } mat4;


// vector 2 double
vec2 vec2_create(double x, double y);
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

// vector 3 double
vec3 vec3_create(double x, double y, double z);
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
void printvec3(char* name, vec3 v);// debug

// vector 4 double
vec4 vec4_create(double x, double y, double z, double w);
vec3 homogenize(vec4 h);

// matrix 2x2 double
mat2 m2add(mat2 m, mat2 n);
mat2 m2sub(mat2 m, mat2 n);
mat2 m2mul(mat2 m, mat2 n);
mat2 m2div(mat2 m, double d);
mat2 m2smul(mat2 m, double s);
mat2 m2scale(vec2 v);
//mat2 trans(vec2 v);
mat2 m2rotate(vec2 v);
mat2 m2shearx(double s);
mat2 m2sheary(double s);
void printmat2(char* name, mat2 m);// debug

// matrix 3x3 double
mat3 m3add(mat3 m, mat3 n);
mat3 m3sub(mat3 m, mat3 n);
mat3 m3mul(mat3 m, mat3 n);
mat3 m3div(mat3 m, double d);
mat3 m3smul(mat3 m, double s);
mat3 m3scale(vec2 v);
mat3 m3trans(vec2 v);
mat3 m3rotate(vec2 v);
void printmat3(char* name, mat3 m);// debug

// matrix 4x4 double
mat4 madd(mat4 m, mat4 n);
mat4 msub(mat4 m, mat4 n);
mat4 mmul(mat4 m, mat4 n);
mat4 mdiv(mat4 m, double d);
mat4 msmul(mat4 m, double s);
mat4 scale(vec3 v);
mat4 trans(vec3 v);
mat4 rotate(vec3 v);
mat4 perspec(double fov, double n, double f, double sw, double sh);
mat4 ortho(double l, double r, double t, double b, double n, double f);
mat4 lookat(vec3 u, vec3 v, vec3 w);
void printmat4(char* name, mat4 m); // debug

#endif