#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

#include "LTK\platform_defines.h"

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

/// @brief Zero filled vec2
vec2 vec2_zero();
/// @brief Zero filled vec3
vec3 vec3_zero();
/// @brief Zero filled vec4
vec4 vec4_zero();

/// @brief One filled vec2
vec2 vec2_one();
/// @brief One filled vec3
vec3 vec3_one();
/// @brief One filled vec4
vec4 vec4_one();

/// @brief Adds two vec2
vec2 vec2_add(vec2 a, vec2 b);
/// @brief Adds two vec3
vec3 vec3_add(vec3 a, vec3 b);
/// @brief Adds two vec4
vec4 vec4_add(vec4 a, vec4 b);

/// @brief Subtracts two vec2
vec2 vec2_sub(vec2 a, vec2 b);
/// @brief Subtracts two vec3
vec3 vec3_sub(vec3 a, vec3 b);
/// @brief Subtracts two vec4
vec4 vec4_sub(vec4 a, vec4 b);

/// @brief Hadamard product of two vec2
vec2 vec2_mul(vec2 a, vec2 b);
/// @brief Hadamard product of two vec3
vec3 vec3_mul(vec3 a, vec3 b);
/// @brief Hadamard product of two vec4
vec4 vec4_mul(vec4 a, vec4 b);

/// @brief Scales a vec2 by s
vec2 vec2_scale(vec2 v, float s);
/// @brief Scales a vec3 by s
vec3 vec3_scale(vec3 v, float s);
/// @brief Scales a vec4 by s
vec4 vec4_scale(vec4 v, float s);

/// @brief Interpolates from a to b by t
vec2 vec2_lerp(vec2 a, vec2 b, float t);
/// @brief Interpolates from a to b by t
vec3 vec3_lerp(vec3 a, vec3 b, float t);
/// @brief Interpolates from a to b by t
vec4 vec4_lerp(vec4 a, vec4 b, float t);

/// @brief The squared euclidian length of v (To avoid needed sqrt)
float vec2_length_sqr(vec2 v);
/// @brief The squared euclidian length of v (To avoid needed sqrt)
float vec3_length_sqr(vec3 v);
/// @brief The squared euclidian length of v (To avoid needed sqrt)
float vec4_length_sqr(vec4 v);


/// @brief The euclidian length of v
float vec2_length(vec2 v);
/// @brief The euclidian length of v
float vec3_length(vec3 v);
/// @brief The euclidian length of v
float vec4_length(vec4 v);

/// @brief Normalized vec2 v
vec2 vec2_normalize(vec2 v);
/// @brief Normalized vec3 v
vec3 vec3_normalize(vec3 v);
/// @brief Normalized vec4 v
vec4 vec4_normalize(vec4 v);

/// @brief Dot product of a and b
float vec2_dot(vec2 a, vec2 b);
/// @brief Dot product of a and b
float vec4_dot(vec4 a, vec4 b);
/// @brief Dot product of a and b
float vec3_dot(vec3 a, vec3 b);

/// @brief Cross product of a and b
vec3 vec3_cross(vec3 a, vec3 b);

/// @brief Filled 2 by 2 identity matrix
mat2 mat2_identity();
/// @brief Filled 3 by 3 identity matrix
mat3 mat3_identity();
/// @brief Filled 4 by 4 identity matrix
mat4 mat4_identity();

/// @brief Matrix vector multiplication of m and v (Aka transformation)
vec2 mat2_vec2_mul(mat2 m, vec2 v);
/// @brief Matrix vector multiplication of m and v (Aka transformation)
vec3 mat3_vec3_mul(mat3 m, vec3 v);
/// @brief Matrix vector multiplication of m and v (Aka transformation)
vec4 mat4_vec4_mul(mat4 m, vec4 v);

/// @brief Matrix matrix multiplication
mat2 mat2_mat2_mul(mat2 a, mat2 b);
/// @brief Matrix matrix multiplication
mat3 mat3_mat3_mul(mat3 a, mat3 b);
/// @brief Matrix matrix multiplication
mat4 mat4_mat4_mul(mat4 a, mat4 b);

#endif // LINEAR_ALGEBRA_H