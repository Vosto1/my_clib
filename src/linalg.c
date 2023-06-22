#include "linalg.h"


vec2 vec2_create(double x, double y)
{
    vec2 v;
    v.x = x;
    v.y = y;
    return v;
}

vec2 v2add(vec2 v, vec2 w)
{
    vec2 a;
    a.x = v.x + w.x;
    a.y = v.y + w.y;
    return a;
}

vec2 v2sub(vec2 v, vec2 w)
{
    vec2 s;
    s.x = v.x - w.x;
    s.y = v.y - w.y;
    return s;
}

vec2 v2mul(vec2 v, vec2 w)
{
    vec2 m;
    m.x = v.x * w.x;
    m.y = v.y * w.y;
    return m;
}

vec2 v2smul(vec2 v, double s)
{
    vec2 m;
    m.x = v.x * s;
    m.y = v.y * s;
    return m;
}

vec2 v2div(vec2 v, double d)
{
    return v2smul(v, 1.0f / d);
}

vec2 homogenize2d(vec3 h)
{
    vec2 v;
    if (h.z == 0)
    {
        fprintf(stderr, "homogenize2d div0\n");
        v.x = v.y = __DBL_MAX__; // "infinity"
    }
    v.x = h.x / h.z;
    v.y = h.y / h.z;
    return v;
}

double v2len(vec2 v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

vec2 v2normalize(vec2 v)
{
    double l = v2len(v);
    vec2 n;
    n.x = v.x / l;
    n.y = v.y / l;
    return n;
}

// parallellogram area
double v2cross(vec2 v, vec2 w)
{
    return v.x * w.y - v.y * w.x;
}

double v2dot(vec2 v, vec2 w)
{
    return v.x * w.x + v.y * w.y;
}

vec3 vec3_create(double x, double y, double z)
{
    vec3 v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

vec3 vadd(vec3 v, vec3 w)
{
    vec3 a;
    a.x = v.x + w.x;
    a.y = v.y + w.y;
    a.z = v.z + w.z;
    return a;
}

vec3 vsub(vec3 v, vec3 w)
{
    vec3 s;
    s.x = v.x - w.x;
    s.y = v.y - w.y;
    s.z = v.z - w.z;
    return s;
}

vec3 cross(vec3 v, vec3 w)
{
    vec3 c;
    c.x = v.y * w.z - v.z * w.y;
    c.y = v.z * w.x - v.x * w.z;
    c.z = v.x * w.y - v.y * w.x;
    return c;
}

double dot(vec3 v, vec3 w)
{
    return v.x * w.x + v.y * w.y + v.z * w.z;
}

double len(vec3 v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3 normalize(vec3 v)
{
    double l = len(v);
    vec3 n;
    n.x = v.x / l;
    n.y = v.y / l;
    n.z = v.z / l;
    return n;
}

vec3 vsmul(vec3 v, double s)
{
    v.x *= s;
    v.y *= s;
    v.z *= s;
    return v;
}

vec4 vmmul(vec3 v, mat4 m)
{
    vec4 h;
    h.x = v.x * m.i[0] + v.y * m.i[4] + v.z * m.i[8] + m.i[12];
    h.y = v.x * m.i[1] + v.y * m.i[5] + v.z * m.i[9] + m.i[13];
    h.z = v.x * m.i[2] + v.y * m.i[6] + v.z * m.i[10] + m.i[14];
    h.w = v.x * m.i[3] + v.y * m.i[7] + v.z * m.i[11] + m.i[15];
    return h;
}

vec3 surface_normal(vec3 u, vec3 v, vec3 w)
{
    return normalize(cross(vsub(u, v), vsub(u, w)));
}

vec3 vrotate(vec3 v, vec3 axis, double angle)
{
    // project vector to rotate (v) to get coincident component
    vec3 coinccomp = vsmul(axis, dot(v, axis) / dot(axis, axis));
    // subtract the concident component from vector to rotate (v) to get the right angled component
    vec3 racomp = vsub(v, coinccomp);
    // cross prod of right angle component and axis to get a vector orthogonal to both
    vec3 orthog = cross(axis, racomp);
    // calculate rotation scalars for the right angled vectors
    double x1 = cos((angle * M_PI) / 180.0f) / len(racomp);
    double x2 = sin((angle * M_PI) / 180.0f) / len(orthog);
    // get the rotated right angled vector
    vec3 rarotated = vsmul(vadd(vsmul(racomp, x1), vsmul(orthog, x2)), len(racomp));
    // assemble the vector components
    return vadd(coinccomp, rarotated);
}

void printvec3(char* name, vec3 v)
{
    printf("%s: %6.5lf %6.5lf %6.5lf\n", name, v.x, v.y, v.z);
}

vec4 vec4_create(double x, double y, double z, double w)
{
    vec4 v;
    v.x = x;
    v.y = y;
    v.z = z;
    v.w = w;
    return v;
}

vec3 homogenize(vec4 h)
{
    vec3 v;
    if (h.w == 0)
    {
        fprintf(stderr, "homogenize div0\n");
        v.x = v.y = v.z = __DBL_MAX__; // "infinity"
    }
    v.x = h.x / h.w;
    v.y = h.y / h.w;
    v.z = h.z / h.w;
    return v;
}

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
    mat2 m2;
    int i, j, k;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
        {
            m.i[j*4+i] = 0.0f;
            for (k = 0; k < 2; k++)
            {
                m2.i[j*2+i] += m.i[k*2+i] * n.i[j*2+i];
            }
        }
    }
    return m2;
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


mat3 m3add(mat3 m, mat3 n)
{
    mat3 t;
    for (int i = 0; i < 9; i++)
    {
        t.i[i] = m.i[i] + n.i[i];
    }
    return t;
}

mat3 m3sub(mat3 m, mat3 n)
{
    mat3 t;
    for (int i = 0; i < 9; i++)
    {
        t.i[i] = m.i[i] - n.i[i];
    }
    return t;
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
    mat3 t;
    for (int i = 0; i < 16; i++)
    {
        t.i[i] = m.i[i] * s;
    }
    return t;
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
            printf("%6.5lf ", m.i[j*3+i]);
        }
        printf("\n");
    }
}


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