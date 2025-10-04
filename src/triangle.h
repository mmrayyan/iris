#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "color.h"
#include "texture.h"
#include "vector.h"
#include <stdint.h>

typedef struct {
  int a;
  int b;
  int c;
  tex2_t a_uv;
  tex2_t b_uv;
  tex2_t c_uv;
  color_t color;
} face_t;

typedef struct {
  vec4_t points[3];
  tex2_t texcoords[3];
  color_t color;
} triangle_t;

void draw_triangle(triangle_t triangle, color_t color);
void draw_filled_triangle(triangle_t triangle);
void draw_textured_triangle(triangle_t triangle, uint32_t *texture);

#endif
