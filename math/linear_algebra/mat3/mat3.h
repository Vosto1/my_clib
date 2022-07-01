#ifndef MAT3_H
#define MAT3_H

#include "../vec2/vec2.h"

typedef struct matrix3x3d { double i[9]; } mat3;

mat3 m3add(mat3 m, mat3 n);
mat3 m3sub(mat3 m, mat3 n);
mat3 m3mul(mat3 m, mat3 n);
mat3 m3div(mat3 m, double d);
mat3 m3smul(mat3 m, double s);

mat3 m3scale(vec2 v);
mat3 m3trans(vec2 v);
mat3 m3rotate(vec2 v);

// debug
void printmat3(char* name, mat3 m);

#endif