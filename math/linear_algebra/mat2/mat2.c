#include "mat2.h"

mat2 m2add(mat2 m, mat2 n)
{
    mat2 a;
    a.i[0] = m.i[0] + n.i[0];
    a.i[1] = m.i[1] + n.i[1];
    a.i[2] = m.i[2] + n.i[2];
    a.i[3] = m.i[3] + n.i[3];
    return a;
}

mat2 m2sub(mat2 m, mat2 n)
{
    mat2 s;
    s.i[0] = m.i[0] - n.i[0];
    s.i[1] = m.i[1] - n.i[1];
    s.i[2] = m.i[2] - n.i[2];
    s.i[3] = m.i[3] - n.i[3];
    return s;
}

mat2 m2mul(mat2 m, mat2 n)
{
    mat2 m;
    int i, j, k;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
        {
            m.i[j*4+i] = 0.0f;
            for (k = 0; k < 2; k++)
            {
                m.i[j*2+i] += m.i[k*2+i] * n.i[j*2+i];
            }
        }
    }
    return m;
}

mat2 m2div(mat2 m, double d)
{
    return m2smul(m, 1.0f / d);
}

mat2 m2smul(mat2 m, double s)
{
    mat2 t;
    for (int i = 0; i < 4; i++)
    {
        t.i[i] = m.i[i] * s;
    }
    return t;
}

mat2 m2scale(vec2 v)
{
    mat2 s;
    s.i[0] = v.x; s.i[2] = 0.0f;
    s.i[1] = 0.0f; s.i[3] = v.y;
    return s;
}

/*mat2 trans(vec2 v) // translation is just a vec2 with 2d matrices
{
    mat2 s;
    s.i[0] = 1.0f; s.i[2] = 0.0f;
    s.i[1] = 0.0f; s.i[3] = 1.0f;
    return s;
}*/

mat2 m2rotate(vec2 v)
{
    double cx = cos((v.x * M_PI) / 180.0f);
    double sx = sin((v.x * M_PI) / 180.0f);
    double cy = cos((v.y * M_PI) / 180.0f);
    double sy = sin((v.y * M_PI) / 180.0f);

    mat2 x;
    x.i[0] = cx; x.i[2] = -sx;
    x.i[1] = sx; x.i[3] = cx;

    mat2 y;
    y.i[0] = cy; y.i[2] = -sy;
    y.i[1] = sy; y.i[3] = cy;

    return m2mul(y, x);
}

mat2 m2shearx(double s)
{
    mat2 sx;
    sx.i[0] = 1.0f; sx.i[2] = s;
    sx.i[1] = 0.0f; sx.i[3] = 1.0f;
    return sx;
}

mat2 m2sheary(double s)
{
    mat2 sy;
    sy.i[0] = 1.0f; sy.i[2] = 0.0f;
    sy.i[1] = s; sy.i[3] = 1.0f;
    return sy;
}

void printmat2(char* name, mat2 m)
{
    int i, j;
    printf("%s:\n", name);
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
        {
            printf("%6.5lf ", m.i[j*2+i]);
        }
        printf("\n");
    }
}

