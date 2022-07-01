#include "mat3.h"

mat3 m3add(mat3 m, mat3 n)
{
    mat3 m;
    for (int i = 0; i < 9; i++)
    {
        m.i[i] = m.i[i] + n.i[i];
    }
    return m;
}

mat3 m3sub(mat3 m, mat3 n)
{
    mat3 m;
    for (int i = 0; i < 9; i++)
    {
        m.i[i] = m.i[i] - n.i[i];
    }
    return m;
}

mat3 m3mul(mat3 m, mat3 n)
{
    mat3 t;
    int i, j, k;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            t.i[j*3+i] = 0.0f;
            for (k = 0; k < 3; k++)
            {
                t.i[j*3+i] += m.i[k*3+i] * n.i[j*3+i];
            }
        }
    }
    return t;
}

mat3 m3div(mat3 m, double d)
{
    return m3smul(m, 1.0f / d);
}

mat3 m3smul(mat3 m, double s)
{
    mat3 m;
    for (int i = 0; i < 16; i++)
    {
        m.i[i] = m.i[i] * s;
    }
    return m;
}

mat3 m3scale(vec2 v)
{
    mat3 s;
    s.i[0] = v.x;  s.i[3] = 0.0f; s.i[6] = 0.0f;
    s.i[1] = 0.0f; s.i[4] = v.y;  s.i[7] = 0.0f;
    s.i[2] = 0.0f; s.i[5] = 0.0f; s.i[8] = 1.0f;
    return s;
}

mat3 m3trans(vec2 v)
{
    mat3 t;
    t.i[0] = 1.0f;  t.i[3] = 0.0f; t.i[6] = v.x;
    t.i[1] = 0.0f; t.i[4] = 1.0f;  t.i[7] = v.y;
    t.i[2] = 0.0f; t.i[5] = 0.0f; t.i[8] = 1.0f;
    return t;
}

mat3 m3rotate(vec2 v)
{
    double cx = cos((v.x * M_PI) / 180.0f);
    double sx = sin((v.x * M_PI) / 180.0f);
    double cy = cos((v.y * M_PI) / 180.0f);
    double sy = sin((v.y * M_PI) / 180.0f);

    mat3 x;
    x.i[0] = cx;   x.i[3] = -sx;  x.i[6] = 0.0f;
    x.i[1] = sx;   x.i[4] = cx;   x.i[7] = 0.0f;
    x.i[2] = 0.0f; x.i[5] = 0.0f; x.i[8] = 1.0f;
    
    mat3 y;
    y.i[0] = cy;   y.i[3] = -sy;  y.i[6] = 0.0f;
    y.i[1] = sy;   y.i[4] = cy;   y.i[7] = 0.0f;
    y.i[2] = 0.0f; y.i[5] = 0.0f; y.i[8] = 1.0f;

    return m3mul(y, x);
}

void printmat3(char* name, mat3 m)
{
    int i, j;
    printf("%s:\n", name);
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            printf("%6.5lf ", m.i[j*4+i]);
        }
        printf("\n");
    }
}

