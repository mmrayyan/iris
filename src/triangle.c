#include "triangle.h"
#include "display.h"
#include "draw.h"
#include <stdint.h>

void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2,
                               color_t color) {
  float inv_slope1 = (x1 - x0) / (float)(y1 - y0);
  float inv_slope2 = (x2 - x0) / (float)(y2 - y0);

  float x_start = x0;
  float x_end = x0;

  for (int y = y0; y <= y2; y++) {
    draw_line(x_start, y, x_end, y, color);
    x_start += inv_slope1;
    x_end += inv_slope2;
  }
}

void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2,
                            color_t color) {
  float inv_slope1 = (x0 - x2) / (float)(y0 - y2);
  float inv_slope2 = (x1 - x2) / (float)(y1 - y2);

  float x_start = x2;
  float x_end = x2;

  for (int y = y2; y >= y0; y--) {
    draw_line(x_start, y, x_end, y, color);
    x_start -= inv_slope1;
    x_end -= inv_slope2;
  }
}

void draw_filled_triangle(triangle_t triangle) {
  int x0 = triangle.points[0].x, y0 = triangle.points[0].y;
  int x1 = triangle.points[1].x, y1 = triangle.points[1].y;
  int x2 = triangle.points[2].x, y2 = triangle.points[2].y;

  if (y0 > y1) {
    swap(&x0, &x1);
    swap(&y0, &y1);
  }
  if (y1 > y2) {
    swap(&x1, &x2);
    swap(&y1, &y2);
  }
  if (y0 > y1) {
    swap(&x0, &x1);
    swap(&y0, &y1);
  }

  if (y1 == y2) {
    fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, triangle.color);
    return;
  }

  if (y0 == y1) {
    fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, triangle.color);
    return;
  }

  int My = y1;
  int Mx = (x2 - x0) * (y1 - y0) / (float)(y2 - y0) + x0;

  fill_flat_bottom_triangle(x0, y0, x1, y1, Mx, My, triangle.color);
  fill_flat_top_triangle(x1, y1, Mx, My, x2, y2, triangle.color);
}

void draw_triangle(triangle_t triangle, color_t color) {
  draw_line(triangle.points[0].x, triangle.points[0].y, triangle.points[1].x,
            triangle.points[1].y, color);
  draw_line(triangle.points[1].x, triangle.points[1].y, triangle.points[2].x,
            triangle.points[2].y, color);
  draw_line(triangle.points[2].x, triangle.points[2].y, triangle.points[0].x,
            triangle.points[0].y, color);
}
