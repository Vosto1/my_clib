#include "mat2.h"

mat2 madd(mat2 m, mat2 n)
{
    mat2 a;
    a.i[0] = m.i[0] + n.i[0];
    a.i[1] = m.i[1] + n.i[1];
    a.i[2] = m.i[2] + n.i[2];
    a.i[3] = m.i[3] + n.i[3];
    return a;
}

mat2 msub(mat2 m, mat2 n)
{
    mat2 s;
    s.i[0] = m.i[0] - n.i[0];
    s.i[1] = m.i[1] - n.i[1];
    s.i[2] = m.i[2] - n.i[2];
    s.i[3] = m.i[3] - n.i[3];
    return s;
}

mat2 mmul(mat2 m, mat2 n)
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

mat2 mdiv(mat2 m, double d)
{
    return msmul(m, 1.0f / d);
}

mat2 msmul(mat2 m, double s)
{
    mat2 t;
    for (int i = 0; i < 4; i++)
    {
        t.i[i] = m.i[i] * s;
    }
    return t;
}
/*
mat2 scale(vec2 v)
{
    mat2 s;
    s.i[0] = v.x; s.i[2] = 0.0f;
    s.i[1] = 0.0f; s.i[3] = v.y;
    return s;
}

mat2 trans(vec2 v)
{
    mat2 s;
    s.i[0] = 1.0f; s.i[2] = 0.0f;
    s.i[1] = 0.0f; s.i[3] = 1.0f;
    return s;
}

mat2 rotate(vec2 v)
{
mat2 s;
    s.i[0] = 1.0f; s.i[2] = 0.0f;
    s.i[1] = 0.0f; s.i[3] = 1.0f;
    return s;
}

void printmat2(char* name, mat2 m)
{

}*/

