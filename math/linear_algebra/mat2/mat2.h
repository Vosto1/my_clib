#ifndef MAT2_H
#define MAT2_H

#include "../vec2/vec2.h"

typedef struct matrix2x2d { double i[4]; } mat2;

mat2 madd(mat2 m, mat2 n);
mat2 msub(mat2 m, mat2 n);
mat2 mmul(mat2 m, mat2 n);
mat2 mdiv(mat2 m, double d);
mat2 msmul(mat2 m, double s);
/*mat2 scale(vec2 v);
mat2 trans(vec2 v);
mat2 rotate(vec2 v);

// debug
void printmat2(char* name, mat2 m);*/

#endif