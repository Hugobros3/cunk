#include "cunk/math.h"

#include <math.h>

Mat4f transpose_mat4f(Mat4f src) {
    return (Mat4f) {
        src.m00, src.m10, src.m20, src.m30,
        src.m01, src.m11, src.m21, src.m31,
        src.m02, src.m12, src.m22, src.m32,
        src.m03, src.m13, src.m23, src.m33,
    };
}

Mat4f look_at_mat4f(Vec3f eye, Vec3f at, Vec3f up) {
    Vec3f dir = vec3f_elemwise_sub(eye, at);
}

Mat4f perspective_mat4f(float a, float fov, float n, float f) {
    float pi = M_PI;
    float s = 1.0f / tanf(fov * 0.5f * (pi / 180.0f));
    return (Mat4f) {
        s / a, 0, 0, 0,
        0, s, 0, 0,
        0, 0, -f / (f - n), -1.f,
        0, 0, - (f * n) / (f - n), 0
    };
}

Mat4f translate_mat4f(Vec3f offset) {
    Mat4f m = identity_mat4f;
    m.m30 = offset.x;
    m.m31 = offset.y;
    m.m32 = offset.z;
    return m;
}

Mat4f rotate_axis_mat4f(unsigned int axis, float f) {
    Mat4f m = { 0 };
    m.m33 = 1;

    unsigned int t = (axis + 2) % 3;
    unsigned int s = (axis + 1) % 3;

    m.rows[t].arr[t] =  cosf(f);
    m.rows[t].arr[s] = -sinf(f);
    m.rows[s].arr[t] =  sinf(f);
    m.rows[s].arr[s] =  cosf(f);

    // leave that unchanged
    m.rows[axis].arr[axis] = 1;

    return m;
}

Mat4f mul_mat4f(Mat4f l, Mat4f r) {
    Mat4f dst = { 0 };
#define a(i, j) m##i##j
#define t(bc, br, i) l.a(i, br) * r.a(bc, i)
#define e(bc, br) dst.a(bc, br) = t(bc, br, 0) + t(bc, br, 1) + t(bc, br, 2) + t(bc, br, 3);
#define row(c) e(c, 0) e(c, 1) e(c, 2) e(c, 3)
#define genmul() row(0) row(1) row(2) row(3)
    genmul()
    return dst;
}
