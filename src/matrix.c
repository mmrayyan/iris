#include "matrix.h"
#include <math.h>
#include <stddef.h>

mat4_t mat4_indentity(void) {
  mat4_t m = {{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
  return m;
}

mat4_t mat4_make_scale(float sx, float sy, float sz) {
  mat4_t m = {{{sx, 0, 0, 0}, {0, sy, 0, 0}, {0, 0, sz, 0}, {0, 0, 0, 1}}};
  return m;
}

mat4_t mat4_make_translation(float tx, float ty, float tz) {
  mat4_t m = {{{1, 0, 0, tx}, {0, 1, 0, ty}, {0, 0, 1, tz}, {0, 0, 0, 1}}};
  return m;
}

mat4_t mat4_make_rotation_x(float angle) {
  float c = cos(angle);
  float s = sin(angle);
  mat4_t m = {{{1, 0, 0, 0}, {0, c, -s, 0}, {0, s, c, 0}, {0, 0, 0, 1}}};
  return m;
}

mat4_t mat4_make_rotation_y(float angle) {
  float c = cos(angle);
  float s = sin(angle);
  mat4_t m = {{{c, 0, s, 0}, {0, 1, 0, 0}, {-s, 0, c, 0}, {0, 0, 0, 1}}};
  return m;
}

mat4_t mat4_make_rotation_z(float angle) {
  float c = cos(angle);
  float s = sin(angle);
  mat4_t m = {{{c, -s, 0, 0}, {s, c, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
  return m;
}

mat4_t mat4_make_perspective(float fov, float aspect, float znear, float zfar) {
  mat4_t m = {{{0}}};
  m.m[0][0] = aspect * (1 / tan(fov / 2));
  m.m[1][1] = 1 / tan(fov / 2);
  m.m[2][2] = zfar / (zfar - znear);
  m.m[2][3] = (-zfar * znear) / (zfar - znear);
  m.m[3][2] = 1;
  return m;
}

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v) {
  vec4_t result;
  result.x =
      m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
  result.y =
      m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
  result.z =
      m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
  result.w =
      m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;

  return result;
}

mat4_t mat4_mul_mat4(mat4_t a, mat4_t b) {
  mat4_t result;

  for (size_t i = 0; i < 4; i++) {
    for (size_t j = 0; j < 4; j++) {
      result.m[i][j] = 0;
      for (size_t k = 0; k < 4; k++) {
        result.m[i][j] += a.m[i][k] * b.m[k][j];
      }
    }
  }

  return result;
}

vec4_t mat4_mul_vec4_project(mat4_t m, vec4_t v) {
  vec4_t result = mat4_mul_vec4(m, v);

  if (result.w != 0.0) {
    result.x /= result.w;
    result.y /= result.w;
    result.z /= result.w;
  }

  return result;
}
