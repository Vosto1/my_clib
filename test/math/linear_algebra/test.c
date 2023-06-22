#include "test.h"

void test()
{
    vec2 a, b;
    vec3 c, d;
    vec4 e, f;
    mat2 g, h;
    mat3 i, j;
    mat4 k, l;

    a = vec2_create(3, 4);
    b = vec2_create(2, 1);

    vec2 ab;

    ab = v2add(b, a);
    assert(ab.x == 5 && ab.y == 5);
    ab = v2sub(a, b);
    assert(ab.x == 1 && ab.y == 3);
    ab = v2mul(a, b);
    assert(ab.x == 6 && ab.y == 4);
    ab = v2smul(a, 2);
    assert(ab.x == 6 && ab.y == 8);
    ab = v2div(b, 2);
    assert(ab.x == 1 && ab.y == 0.5);
    vec3 x = vec3_create(2, 4, 2);
    ab = homogenize2d(x);
    assert(ab.x == 1 && ab.y == 2);
    assert(v2len(a) == 5);
    ab = v2normalize(a);
    assert(ab.x == 0.6 && ab.y == 0.8);
    assert(v2cross(a, b) == -5);
    assert(v2dot(a, b) == 10);

    c = vec3_create(4, 2, 6);
    d = vec3_create(1, 0, -1);

    vec3 cd;

    cd = vadd(c, d);
    assert(cd.x == 5 && cd.y == 2 && cd.z == 5);
    cd = vsub(c, d);
    assert(cd.x == 3 && cd.y == 2 && cd.z == 7);
    cd = cross(c, d);
    assert(cd.x == -2 && cd.y == 10 && cd.z == -2);
    assert(dot(c, d) == -2);
    assert(len(c) == 2*sqrt(14));
    cd = normalize(c);
    //printvec3("idk", cd);
    //printf("%lf ", 4/(2*sqrt(14)));
    //printf("%lf ", 2/(2*sqrt(14)));
    //printf("%lf ", 6/(2*sqrt(14)));
    assert(cd.x == 4/(2*sqrt(14)) && cd.y == 2/(2*sqrt(14)) && cd.z == 6/(2*sqrt(14)));
    cd = vsmul(c, 2);
    assert(cd.x == 8 && cd.y == 4 && cd.z == 12);
}