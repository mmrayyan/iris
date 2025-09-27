#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "color.h"
#include "vector.h"
#include <stdint.h>

typedef struct {
  int a;
  int b;
  int c;
  color_t color;
} face_t;

typedef struct {
  vec2_t points[3];
  color_t color;
  float average_depth;
} triangle_t;

void draw_filled_triangle(triangle_t triangle);
void draw_triangle(triangle_t triangle, color_t color);

#endif
