#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

#include <math.h>

typedef struct vec2
{
    float x, y;
} vec2;

typedef struct vec3
{
    float x, y, z;
} vec3;

typedef struct vec4
{
    float x, y, z, w;
} vec4;

typedef struct mat2
{
    float m00, m10;
    float m01, m11;
} mat2;

typedef struct mat3
{
    float m00, m10, m20;
    float m01, m11, m21;
    float m02, m12, m22;
} mat3;

typedef struct mat4
{
    float m00, m10, m20, m30;
    float m01, m11, m21, m31;
    float m02, m12, m22, m32;
    float m03, m13, m23, m33;
} mat4;

#define VEC2_ZERO (vec2){0}
#define VEC3_ZERO (vec3){0}
#define VEC4_ZERO (vec4){0}

#define VEC2_ONE (vec2){1.f, 1.f}
#define VEC3_ONE (vec3){1.f, 1.f, 1.f}
#define VEC4_ONE (vec4){1.f, 1.f, 1.f, 1.f}

#define MAT2_IDENTITY\
    (mat2){\
        1.f, 0.f,\
        0.f, 1.f,\
    }

#define MAT3_IDENTITY\
    (mat3){\
        1.f, 0.f, 0.f,\
        0.f, 1.f, 0.f,\
        0.f, 0.f, 1.f,\
    }

#define MAT4_IDENTITY\
    (mat4){\
        1.f, 0.f, 0.f, 0.f,\
        0.f, 1.f, 0.f, 0.f,\
        0.f, 0.f, 1.f, 0.f,\
        0.f, 0.f, 0.f, 1.f,\
    }

/// @brief Adds two vec2
static inline vec2 vec2_add(vec2 a, vec2 b)
{
    return (vec2){
        a.x + b.x, 
        a.y + b.y,
    };
}

/// @brief Adds two vec3
static inline vec3 vec3_add(vec3 a, vec3 b)
{
    return (vec3){
        a.x + b.x, 
        a.y + b.y, 
        a.z + b.z,
    };
}

/// @brief Adds two vec4
static inline vec4 vec4_add(vec4 a, vec4 b)
{
    return (vec4){
        a.x + b.x, 
        a.y + b.y, 
        a.z + b.z, 
        a.w + b.w,
    };
}

/// @brief Subtracts two vec2
static inline vec2 vec2_sub(vec2 a, vec2 b)
{
    return (vec2){
        a.x - b.x, 
        a.y - b.y,
    };
}

/// @brief Subtracts two vec3
static inline vec3 vec3_sub(vec3 a, vec3 b)
{
    return (vec3){
        a.x - b.x, 
        a.y - b.y, 
        a.z - b.z,
    };
}

/// @brief Subtracts two vec4
static inline vec4 vec4_sub(vec4 a, vec4 b)
{
    return (vec4){
        a.x - b.x, 
        a.y - b.y, 
        a.z - b.z, 
        a.w - b.w,
    };
}

/// @brief Hadamard product of two vec2
static inline vec2 vec2_mul(vec2 a, vec2 b)
{
    return (vec2){
        a.x * b.x, 
        a.y * b.y,
    };
}

/// @brief Hadamard product of two vec3
static inline vec3 vec3_mul(vec3 a, vec3 b)
{
    return (vec3){
        a.x * b.x, 
        a.y * b.y, 
        a.z * b.z,
    };
}

/// @brief Hadamard product of two vec4
static inline vec4 vec4_mul(vec4 a, vec4 b)
{
    return (vec4){
        a.x * b.x, 
        a.y * b.y, 
        a.z * b.z, 
        a.w * b.w,
    };
}

/// @brief Scales a vec2 by s
static inline vec2 vec2_scale(vec2 v, float s)
{
    return (vec2){
        v.x * s,
        v.y * s,
    };
}

/// @brief Scales a vec3 by s
static inline vec3 vec3_scale(vec3 v, float s)
{
    return (vec3){
        v.x * s,
        v.y * s,
        v.z * s,
    };
}

/// @brief Scales a vec4 by s
static inline vec4 vec4_scale(vec4 v, float s)
{
    return (vec4){
        v.x * s,
        v.y * s,
        v.z * s,
        v.w * s,
    };
}

/// @brief Interpolates from a to b by t
static inline vec2 vec2_lerp(vec2 a, vec2 b, float t)
{
    return vec2_add(a, vec2_scale(vec2_sub(b, a), t));
}

/// @brief Interpolates from a to b by t
static inline vec3 vec3_lerp(vec3 a, vec3 b, float t)
{
    return vec3_add(a, vec3_scale(vec3_sub(b, a), t));
}

/// @brief Interpolates from a to b by t
static inline vec4 vec4_lerp(vec4 a, vec4 b, float t)
{
    return vec4_add(a, vec4_scale(vec4_sub(b, a), t));
}

/// @brief The squared euclidian length of v (To avoid needed sqrt)
static inline float vec2_length_sqr(vec2 v)
{
    return v.x * v.x + v.y * v.y;
}

/// @brief The squared euclidian length of v (To avoid needed sqrt)
static inline float vec3_length_sqr(vec3 v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

/// @brief The squared euclidian length of v (To avoid needed sqrt)
static inline float vec4_length_sqr(vec4 v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
}

/// @brief The euclidian length of v
static inline float vec2_length(vec2 v)
{
    return sqrtf(vec2_length_sqr(v));
}

/// @brief The euclidian length of v
static inline float vec3_length(vec3 v)
{
    return sqrtf(vec3_length_sqr(v));
}

/// @brief The euclidian length of v
static inline float vec4_length(vec4 v)
{
    return sqrtf(vec4_length_sqr(v));
}

/// @brief Normalized vec2 v
static inline vec2 vec2_normalize(vec2 v)
{
    float vLen = vec2_length(v);
    return (vec2){
        v.x / vLen, 
        v.y / vLen,
    };
}

/// @brief Normalized vec3 v
static inline vec3 vec3_normalize(vec3 v)
{
    float vLen = vec3_length(v);
    return (vec3){
        v.x / vLen, 
        v.y / vLen, 
        v.z / vLen,
    };
}

/// @brief Normalized vec4 v
static inline vec4 vec4_normalize(vec4 v)
{
    float vLen = vec4_length(v);
    return (vec4){
        v.x / vLen, 
        v.y / vLen, 
        v.z / vLen, 
        v.w / vLen,
    };
}

/// @brief Dot product of a and b
static inline float vec2_dot(vec2 a, vec2 b)
{
    return a.x * b.x + a.y * b.y;
}

/// @brief Dot product of a and b
static inline float vec3_dot(vec3 a, vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

/// @brief Dot product of a and b
static inline float vec4_dot(vec4 a, vec4 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w + b.w;
}

/// @brief Cross product of a and b
static inline vec3 vec3_cross(vec3 a, vec3 b)
{
    return (vec3){
        a.y * b.z - b.y * a.z,
        a.z * b.x - b.z * a.x,
        a.x * b.y - b.x * a.y,
    };
}

/// @brief Matrix vector multiplication of m and v (Aka transformation)
static inline vec2 mat2_vec2_mul(mat2 m, vec2 v)
{
    return (vec2){
        m.m00 * v.x + m.m10 * v.y,
        m.m01 * v.x + m.m11 * v.y,
    };
}

/// @brief Matrix vector multiplication of m and v (Aka transformation)
static inline vec3 mat3_vec3_mul(mat3 m, vec3 v)
{
    return (vec3){
        m.m00 * v.x + m.m10 * v.y + m.m20 * v.z,
        m.m01 * v.x + m.m11 * v.y + m.m21 * v.z,
        m.m02 * v.x + m.m12 * v.y + m.m22 * v.z,
    };
}

/// @brief Matrix vector multiplication of m and v (Aka transformation)
static inline vec4 mat4_vec4_mul(mat4 m, vec4 v)
{
    return (vec4){
        m.m00 * v.x + m.m10 * v.y + m.m20 * v.z + m.m30 * v.w,
        m.m01 * v.x + m.m11 * v.y + m.m21 * v.z + m.m31 * v.w,
        m.m02 * v.x + m.m12 * v.y + m.m22 * v.z + m.m32 * v.w,
        m.m03 * v.x + m.m13 * v.y + m.m23 * v.z + m.m33 * v.w,
    };
}

/// @brief Matrix matrix multiplication
static inline mat2 mat2_mat2_mul(mat2 a, mat2 b)
{
    return (mat2){
        a.m00 * b.m00 + a.m10 * b.m01, 
        a.m00 * b.m10 + a.m10 * b.m11,
        a.m01 * b.m00 + a.m11 * b.m01, 
        a.m01 * b.m10 + a.m11 * b.m11,
    };
}

/// @brief Matrix matrix multiplication
static inline mat3 mat3_mat3_mul(mat3 a, mat3 b)
{
    return (mat3){
        a.m00 * b.m00 + a.m10 * b.m01 + a.m20 * b.m02,
        a.m00 * b.m10 + a.m10 * b.m11 + a.m20 * b.m12,
        a.m00 * b.m20 + a.m10 * b.m21 + a.m20 * b.m22,
        a.m01 * b.m00 + a.m11 * b.m01 + a.m21 * b.m02,
        a.m01 * b.m10 + a.m11 * b.m11 + a.m21 * b.m12,
        a.m01 * b.m20 + a.m11 * b.m21 + a.m21 * b.m22,
        a.m02 * b.m10 + a.m12 * b.m11 + a.m22 * b.m12,
        a.m02 * b.m00 + a.m12 * b.m01 + a.m22 * b.m02,
        a.m02 * b.m20 + a.m12 * b.m21 + a.m22 * b.m22,
    };
}

/// @brief Matrix matrix multiplication
static inline mat4 mat4_mat4_mul(mat4 a, mat4 b)
{
    return (mat4){
        a.m00 * b.m00 + a.m10 * b.m01 + a.m20 * b.m02 + a.m30 * b.m03,
        a.m00 * b.m10 + a.m10 * b.m11 + a.m20 * b.m12 + a.m30 * b.m13, 
        a.m00 * b.m20 + a.m10 * b.m21 + a.m20 * b.m22 + a.m30 * b.m23,
        a.m00 * b.m30 + a.m10 * b.m31 + a.m20 * b.m32 + a.m30 * b.m33,
        a.m01 * b.m00 + a.m11 * b.m01 + a.m21 * b.m02 + a.m31 * b.m03,
        a.m01 * b.m10 + a.m11 * b.m11 + a.m21 * b.m12 + a.m31 * b.m13,
        a.m01 * b.m20 + a.m11 * b.m21 + a.m21 * b.m22 + a.m31 * b.m23,
        a.m01 * b.m30 + a.m11 * b.m31 + a.m21 * b.m32 + a.m31 * b.m33,
        a.m02 * b.m00 + a.m12 * b.m01 + a.m22 * b.m02 + a.m32 * b.m03,
        a.m02 * b.m10 + a.m12 * b.m11 + a.m22 * b.m12 + a.m32 * b.m13,
        a.m02 * b.m20 + a.m12 * b.m21 + a.m22 * b.m22 + a.m32 * b.m23,
        a.m02 * b.m30 + a.m12 * b.m31 + a.m22 * b.m32 + a.m32 * b.m33,
        a.m03 * b.m00 + a.m13 * b.m01 + a.m23 * b.m02 + a.m33 * b.m03,
        a.m03 * b.m10 + a.m13 * b.m11 + a.m23 * b.m12 + a.m33 * b.m13,
        a.m03 * b.m20 + a.m13 * b.m21 + a.m23 * b.m22 + a.m33 * b.m23,
        a.m03 * b.m30 + a.m13 * b.m31 + a.m23 * b.m32 + a.m33 * b.m33,
    };
}

#endif // LINEAR_ALGEBRA_H