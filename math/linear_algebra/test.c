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
    assert(ab.x = 1 && ab.y == -3);
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
    assert(v2dot(a, b) == 11);
}