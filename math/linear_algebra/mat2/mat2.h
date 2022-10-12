#ifndef MAT2_H
#define MAT2_H

#include "../vec2/vec2.h"
#include "../math2.h"

typedef struct matrix2x2d { double i[4]; } mat2;

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

// debug
void printmat2(char* name, mat2 m);

#endif