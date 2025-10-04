#include "triangle.h"
#include "display.h"
#include "draw.h"
#include "swap.h"
#include <stdint.h>

float clamp(float value, float min, float max) {
  if (value < min)
    return min;
  if (value > max)
    return max;
  return value;
}

vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p) {
  vec2_t ac = vec2_sub(c, a);
  vec2_t ab = vec2_sub(b, a);
  vec2_t ap = vec2_sub(p, a);
  vec2_t pc = vec2_sub(c, p);
  vec2_t pb = vec2_sub(b, p);

  float area_parallelogram_abc = ac.x * ab.y - ac.y * ab.x; // || AC x AB ||

  float alpha = (pc.x * pb.y - pc.y * pb.x) / area_parallelogram_abc;
  float beta = (ac.x * ap.y - ac.y * ap.x) / area_parallelogram_abc;
  float gamma = 1 - alpha - beta;

  vec3_t weights = {alpha, beta, gamma};
  return weights;
}

// FILLED TRIANGLE

void draw_filled_pixel(int x, int y, triangle_t triangle) {
  vec4_t point_a = triangle.points[0];
  vec4_t point_b = triangle.points[1];
  vec4_t point_c = triangle.points[2];

  vec2_t a = vec2_from_vec4(triangle.points[0]);
  vec2_t b = vec2_from_vec4(triangle.points[1]);
  vec2_t c = vec2_from_vec4(triangle.points[2]);
  vec2_t p = {x, y};
  vec3_t weights = barycentric_weights(a, b, c, p);

  float alpha = weights.x;
  float beta = weights.y;
  float gamma = weights.z;

  float interpolated_1_w = (1 / point_a.w) * alpha + (1 / point_b.w) * beta +
                           (1 / point_c.w) * gamma;

  interpolated_1_w = 1.0 - interpolated_1_w;

  if (interpolated_1_w < z_buffer[(window_width * y) + x]) {
    draw_pixel(x, y, triangle.color);
    z_buffer[(window_width * y) + x] = interpolated_1_w;
  }
}

void draw_filled_triangle(triangle_t triangle) {
  int x0 = triangle.points[0].x, y0 = triangle.points[0].y;
  int x1 = triangle.points[1].x, y1 = triangle.points[1].y;
  int x2 = triangle.points[2].x, y2 = triangle.points[2].y;
  float z0 = triangle.points[0].z, w0 = triangle.points[0].w;
  float z1 = triangle.points[1].z, w1 = triangle.points[1].w;
  float z2 = triangle.points[2].z, w2 = triangle.points[2].w;

  // Sort vertices (y0 < y1 < y2)
  if (y0 > y1) {
    swap_int(&x0, &x1);
    swap_int(&y0, &y1);
    swap_float(&z0, &z1);
    swap_float(&w0, &w1);
  }
  if (y1 > y2) {
    swap_int(&x1, &x2);
    swap_int(&y1, &y2);
    swap_float(&z1, &z2);
    swap_float(&w1, &w2);
  }
  if (y0 > y1) {
    swap_int(&x0, &x1);
    swap_int(&y0, &y1);
    swap_float(&z0, &z1);
    swap_float(&w0, &w1);
  }

  // Render flat-bottom filled triangle
  float inv_slope_1 = 0;
  float inv_slope_2 = 0;

  if (y1 - y0 != 0) {
    inv_slope_1 = (float)(x1 - x0) / abs(y1 - y0);
  }
  if (y2 - y0 != 0) {
    inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);
  }

  if (y1 - y0 != 0) {
    for (int y = y0; y <= y1; y++) {
      int x_start = x1 + (y - y1) * inv_slope_1;
      int x_end = x0 + (y - y0) * inv_slope_2;

      if (x_end < x_start) {
        swap_int(&x_start, &x_end);
      }

      for (int x = x_start; x <= x_end; x++) {
        draw_filled_pixel(x, y, triangle);
      }
    }
  }

  // Render flat-top filled triangle
  inv_slope_1 = 0;
  inv_slope_2 = 0;

  if (y2 - y1 != 0) {
    inv_slope_1 = (float)(x2 - x1) / abs(y2 - y1);
  }
  if (y2 - y0 != 0) {
    inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);
  }

  if (y2 - y1 != 0) {
    for (int y = y1; y <= y2; y++) {
      int x_start = x1 + (y - y1) * inv_slope_1;
      int x_end = x0 + (y - y0) * inv_slope_2;

      if (x_end < x_start) {
        swap_int(&x_start, &x_end);
      }

      for (int x = x_start; x <= x_end; x++) {
        draw_filled_pixel(x, y, triangle);
      }
    }
  }
}

// OUTLINED TRIANGLE

void draw_triangle(triangle_t triangle, color_t color) {
  draw_line(triangle.points[0].x, triangle.points[0].y, triangle.points[1].x,
            triangle.points[1].y, color);
  draw_line(triangle.points[1].x, triangle.points[1].y, triangle.points[2].x,
            triangle.points[2].y, color);
  draw_line(triangle.points[2].x, triangle.points[2].y, triangle.points[0].x,
            triangle.points[0].y, color);
}

// TEXTURED TRIANGLE

void draw_texel(int x, int y, triangle_t triangle, uint32_t *texture) {
  vec4_t point_a = triangle.points[0];
  vec4_t point_b = triangle.points[1];
  vec4_t point_c = triangle.points[2];
  tex2_t coord_a = triangle.texcoords[0];
  tex2_t coord_b = triangle.texcoords[1];
  tex2_t coord_c = triangle.texcoords[2];

  vec2_t a = vec2_from_vec4(triangle.points[0]);
  vec2_t b = vec2_from_vec4(triangle.points[1]);
  vec2_t c = vec2_from_vec4(triangle.points[2]);
  vec2_t p = {x, y};
  vec3_t weights = barycentric_weights(a, b, c, p);

  float alpha = weights.x;
  float beta = weights.y;
  float gamma = weights.z;

  float interpolated_u = (coord_a.u / point_a.w) * alpha +
                         (coord_b.u / point_b.w) * beta +
                         (coord_c.u / point_c.w) * gamma;

  float interpolated_v = (coord_a.v / point_a.w) * alpha +
                         (coord_b.v / point_b.w) * beta +
                         (coord_c.v / point_c.w) * gamma;

  float interpolated_1_w = (1 / point_a.w) * alpha + (1 / point_b.w) * beta +
                           (1 / point_c.w) * gamma;

  interpolated_u /= interpolated_1_w;
  interpolated_v /= interpolated_1_w;

  int tex_x = abs(clamp(interpolated_u, 0, 1) * texture_width);
  int tex_y = abs(clamp(1 - interpolated_v, 0, 1) * texture_height);

  interpolated_1_w = 1.0 - interpolated_1_w;

  if (interpolated_1_w < z_buffer[(window_width * y) + x]) {
    draw_pixel(x, y, texture[tex_y * texture_width + tex_x]);
    z_buffer[(window_width * y) + x] = interpolated_1_w;
  }
}

void draw_textured_triangle(triangle_t triangle, uint32_t *texture) {
  int x0 = triangle.points[0].x, y0 = triangle.points[0].y;
  int x1 = triangle.points[1].x, y1 = triangle.points[1].y;
  int x2 = triangle.points[2].x, y2 = triangle.points[2].y;
  float z0 = triangle.points[0].z, w0 = triangle.points[0].w;
  float z1 = triangle.points[1].z, w1 = triangle.points[1].w;
  float z2 = triangle.points[2].z, w2 = triangle.points[2].w;
  float u0 = triangle.texcoords[0].u, v0 = triangle.texcoords[0].v;
  float u1 = triangle.texcoords[1].u, v1 = triangle.texcoords[1].v;
  float u2 = triangle.texcoords[2].u, v2 = triangle.texcoords[2].v;

  // Sort vertices (y0 < y1 < y2)
  if (y0 > y1) {
    swap_int(&x0, &x1);
    swap_int(&y0, &y1);
    swap_float(&z0, &z1);
    swap_float(&w0, &w1);
    swap_float(&u0, &u1);
    swap_float(&v0, &v1);
  }
  if (y1 > y2) {
    swap_int(&x1, &x2);
    swap_int(&y1, &y2);
    swap_float(&z1, &z2);
    swap_float(&w1, &w2);
    swap_float(&u1, &u2);
    swap_float(&v1, &v2);
  }
  if (y0 > y1) {
    swap_int(&x0, &x1);
    swap_int(&y0, &y1);
    swap_float(&z0, &z1);
    swap_float(&w0, &w1);
    swap_float(&u0, &u1);
    swap_float(&v0, &v1);
  }

  // Render flat-bottom textured triangle
  float inv_slope_1 = 0;
  float inv_slope_2 = 0;

  if (y1 - y0 != 0) {
    inv_slope_1 = (float)(x1 - x0) / abs(y1 - y0);
  }
  if (y2 - y0 != 0) {
    inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);
  }

  if (y1 - y0 != 0) {
    for (int y = y0; y <= y1; y++) {
      int x_start = x1 + (y - y1) * inv_slope_1;
      int x_end = x0 + (y - y0) * inv_slope_2;

      if (x_end < x_start) {
        swap_int(&x_start, &x_end);
      }

      for (int x = x_start; x <= x_end; x++) {
        draw_texel(x, y, triangle, texture);
      }
    }
  }

  // Render flat-top textured triangle
  inv_slope_1 = 0;
  inv_slope_2 = 0;

  if (y2 - y1 != 0) {
    inv_slope_1 = (float)(x2 - x1) / abs(y2 - y1);
  }
  if (y2 - y0 != 0) {
    inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);
  }

  if (y2 - y1 != 0) {
    for (int y = y1; y <= y2; y++) {
      int x_start = x1 + (y - y1) * inv_slope_1;
      int x_end = x0 + (y - y0) * inv_slope_2;

      if (x_end < x_start) {
        swap_int(&x_start, &x_end);
      }

      for (int x = x_start; x <= x_end; x++) {
        draw_texel(x, y, triangle, texture);
      }
    }
  }
}
