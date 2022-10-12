#ifndef MAT4_H
#define MAT4_H


#include <stdio.h>
#include "../vec3/vec3.h"
#include "../math2.h"

typedef struct matrix4x4d { double i[16]; } mat4;

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

// debug
void printmat4(char* name, mat4 m);

#endif