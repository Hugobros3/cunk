#ifndef CUNK_MATH
#define CUNK_MATH

#define MATH_FN_QUALIFIERS static inline __attribute__ ((const))

#define impl_op_ctor f
#define impl_op_scale ((a.arr[i]) * scale)

#define impl_op_binop_helper(op) impl_op_##op(a.arr[i], b.arr[i])
#define impl_op_add(a, b) ((a) + (b))
#define impl_op_sub(a, b) ((a) - (b))
#define impl_op_mul(a, b) ((a) * (b))
#define impl_op_div(a, b) ((a) / (b))

#define impl_op_unop_helper(op) impl_op_##op(a.arr[i])
#define impl_op_neg(a) (-(a))

#define impl_vec_op(size, snake_name, type_name, op_name, op, ...) \
MATH_FN_QUALIFIERS type_name snake_name##_##op_name(__VA_ARGS__) { \
    type_name thing;                                               \
    for (int i = 0; i < size; i++)                                 \
      thing.arr[i] = impl_op_##op;                                 \
    return thing;                                                  \
}

#define impl_vec_bin_op(size, sn, n, opn, op) impl_vec_op(size, sn, n, opn, binop_helper(op), n a, n b)
#define impl_vec_un_op(size, sn, n, opn, op) impl_vec_op(size, sn, n, opn, unop_helper(op), n a)

#define impl_vec_ops(size, snake_name, name, scalar_name) \
impl_vec_bin_op(size, snake_name, name, add, add) \
impl_vec_bin_op(size, snake_name, name, sub, sub) \
impl_vec_bin_op(size, snake_name, name, mul, mul) \
impl_vec_bin_op(size, snake_name, name, div, div) \
impl_vec_un_op(size, snake_name, name, neg, neg) \
impl_vec_op(size, snake_name, name, scale, scale, name a, scalar_name scale) \
impl_vec_op(size, snake_name, name, ctor, ctor, scalar_name f)

typedef union {
    struct {
        float x, y;
    };
    float arr[2];
} Vec2f;

impl_vec_ops(2, vec2f, Vec2f, float)

typedef union {
    struct {
        float x, y, z;
    };
    float arr[3];
    Vec2f xy;
} Vec3f;

impl_vec_ops(3, vec3f, Vec3f, float)

typedef union {
    struct {
        float x, y, z, w;
    };
    float arr[4];
    Vec2f xy;
    Vec3f xyz;
} Vec4f;

impl_vec_ops(4, vec4f, Vec4f, float)

typedef union {
    struct {
        // we use row-major ordering
        float m00, m01, m02, m03,
              m10, m11, m12, m13,
              m20, m21, m22, m23,
              m30, m31, m32, m33;
    };
    Vec4f rows[4];
    float arr[16];
} Mat4f;

static const Mat4f identity_mat4f = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
};

Mat4f transpose_mat4f(Mat4f);
Mat4f invert_mat4(Mat4f);
Mat4f mul_mat4f(Mat4f, Mat4f);
Vec4f mul_mat4f_vec4f(Mat4f l, Vec4f r);

Mat4f translate_mat4f(Vec3f offset);
Mat4f rotate_axis_mat4f(unsigned int axis, float);
Mat4f perspective_mat4f(float aspect_ratio, float fov, float near, float far);

#endif
