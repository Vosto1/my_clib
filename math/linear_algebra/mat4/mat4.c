#include "mat4.h"

mat4 madd(mat4 m, mat4 n)
{
    mat4 t;
    for (int i = 0; i < 16; i++)
    {
        t.i[i] = m.i[i] + n.i[i];
    }
    return t;
}

mat4 msub(mat4 m, mat4 n)
{
    mat4 t;
    for (int i = 0; i < 16; i++)
    {
        t.i[i] = m.i[i] - n.i[i];
    }
    return t;
}

mat4 mmul(mat4 m, mat4 n)
{
    mat4 t;
    int i, j, k;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            t.i[j*4+i] = 0.0f;
            for (k = 0; k < 4; k++)
            {
                t.i[j*4+i] += m.i[k*4+i] * n.i[j*4+i];
            }
        }
    }
    return t;
}

mat4 mdiv(mat4 m, double d)
{
    return msmul(m, 1.0f / d);
}

mat4 msmul(mat4 m, double s)
{
    mat4 t;
    for (int i = 0; i < 16; i++)
    {
        t.i[i] = m.i[i] * s;
    }
    return t;
}

mat4 scale(vec3 v)
{
    mat4 s;
    s.i[0] = v.x;  s.i[4] = 0.0f; s.i[8] = 0.0f; s.i[12] = 0.0f;
    s.i[1] = 0.0f; s.i[5] = v.y;  s.i[9] = 0.0f; s.i[13] = 0.0f;
    s.i[2] = 0.0f; s.i[6] = 0.0f; s.i[10] = v.z;  s.i[14] = 0.0f;
    s.i[3] = 0.0f; s.i[7] = 0.0f; s.i[11] = 0.0f; s.i[15] = 1.0f;
    return s;
}

mat4 trans(vec3 v)
{
    mat4 t;
    t.i[0] = 1.0f;  t.i[4] = 0.0f; t.i[8] = 0.0f; t.i[12] = v.x;
    t.i[1] = 0.0f; t.i[5] = 1.0f;  t.i[9] = 0.0f; t.i[13] = v.y;
    t.i[2] = 0.0f; t.i[6] = 0.0f; t.i[10] = 1.0f;  t.i[14] = v.z;
    t.i[3] = 0.0f; t.i[7] = 0.0f; t.i[11] = 0.0f; t.i[15] = 1.0f;
    return t;
}

mat4 rotate(vec3 v)
{
    double cx = cos((v.x * M_PI) / 180.0f);
    double sx = sin((v.x * M_PI) / 180.0f);
    double cy = cos((v.y * M_PI) / 180.0f);
    double sy = sin((v.y * M_PI) / 180.0f);
    double cz = cos((v.z * M_PI) / 180.0f);
    double sz = sin((v.z * M_PI) / 180.0f);

    mat4 x;
    x.i[0] = 1.0f; x.i[4] = 0.0f; x.i[8] = 0.0f;  x.i[12] = 0.0f;
    x.i[1] = 0.0f; x.i[5] = cx;   x.i[9] = -cx;   x.i[13] = 0.0f;
    x.i[2] = 0.0f; x.i[6] = sx;   x.i[10] = sx;   x.i[14] = 0.0f;
    x.i[3] = 0.0f; x.i[7] = 0.0f; x.i[11] = 0.0f; x.i[15] = 1.0f;

    mat4 y;
    y.i[0] = cy;   y.i[4] = 0.0f; y.i[8] = sy;    y.i[12] = 0.0f;
    y.i[1] = 0.0f; y.i[5] = 1.0f; y.i[9] = 0.0f;  y.i[13] = 0.0f;
    y.i[2] = -sy;  y.i[6] = 0.0f; y.i[10] = cy;   y.i[14] = 0.0f;
    y.i[3] = 0.0f; y.i[7] = 0.0f; y.i[11] = 0.0f; y.i[15] = 1.0f;

    mat4 z;
    z.i[0] = cz;   z.i[4] = -sz;  z.i[8] = 0.0f;  z.i[12] = 0.0f;
    z.i[1] = sz;   z.i[5] = cz;   z.i[9] = 0.0f;  z.i[13] = 0.0f;
    z.i[2] = 0.0f; z.i[6] = 0.0f; z.i[10] = 1.0f; z.i[14] = 0.0f;
    z.i[3] = 0.0f; z.i[7] = 0.0f; z.i[11] = 0.0f; z.i[15] = 1.0f;

    return mmul(z, mmul(y, x));
}

mat4 perspec(double fov, double n, double f, double sw, double sh)
{
    double fovydiv2rad = ((fov * M_PI) / 180.0f) / 2.0f;
    double cotanfovy = cos(fovydiv2rad) / sin(fovydiv2rad);
    double aspectratio = sw / sh;
    double neg1 = -1.0f;

    mat4 p;
    p.i[0] = cotanfovy / aspectratio; p.i[4] = 0.0f;      p.i[8] = 0.0f;               p.i[12] = 0.0f;
    p.i[1] = 0.0f;                    p.i[5] = cotanfovy; p.i[9] = 0.0f;               p.i[13] = 0.0f;
    p.i[2] = 0.0f;                    p.i[6] = 0.0f;      p.i[10] = (f + n) / (n - f); p.i[14] = (2 * f * n) / (n - f);
    p.i[3] = 0.0f;                    p.i[7] = 0.0f;      p.i[11] = neg1;              p.i[15] = 1.0f;
    return p;
}

mat4 ortho(double l, double r, double t, double b, double n, double f)
{

    mat4 o;
    o.i[0] = 2.0f / (r - l); o.i[4] = 0.0f;           o.i[8] = 0.0f;            o.i[12] = (r + l) / (r - l);
    o.i[1] = 0.0f;           o.i[5] = 2.0f / (t - b); o.i[9] = 0.0f;            o.i[13] = (t + b) / (t - b);
    o.i[2] = 0.0f;           o.i[6] = 0.0f;           o.i[10] = 2.0f / (n - f); o.i[14] = (n + f) / (n - f);
    o.i[3] = 0.0f;           o.i[7] = 0.0f;           o.i[11] = 0.0f;           o.i[15] = 1.0f;
    return o;
}

mat4 lookat(vec3 e, vec3 a, vec3 u)
{
    mat4 l;
    l.i[0] = e.x;  l.i[4] = e.y;  l.i[8] = e.z;   l.i[12] = 0.0f;
    l.i[1] = a.x;  l.i[5] = a.y;  l.i[9] = a.z;   l.i[13] = 0.0f;
    l.i[2] = u.x;  l.i[6] = u.y;  l.i[10] = u.z;  l.i[14] = 0.0f;
    l.i[3] = 0.0f; l.i[7] = 0.0f; l.i[11] = 0.0f; l.i[15] = 1.0f;
    return l;
}

// debug
void printmat4(char* name, mat4 m)
{
    int i, j;
    printf("%s:\n", name);
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            printf("%6.5lf ", m.i[j*4+i]);
        }
        printf("\n");
    }
}