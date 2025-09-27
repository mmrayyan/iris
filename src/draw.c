#include "draw.h"
#include "display.h"

void draw_pixel(int x, int y, color_t color) {
  if (x >= 0 && x < window_width && y >= 0 && y < window_height) {
    color_buffer[(window_width * y) + x] = color;
  }
}

void draw_line(int x0, int y0, int x1, int y1, color_t color) {
  int delta_x = x1 - x0;
  int delta_y = y1 - y0;

  int longest_side_length =
      (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);

  float x_inc = delta_x / (float)longest_side_length;
  float y_inc = delta_y / (float)longest_side_length;

  float current_x = x0;
  float current_y = y0;

  for (int i = 0; i <= longest_side_length; i++) {
    draw_pixel(current_x, current_y, color);
    current_x += x_inc;
    current_y += y_inc;
  }
}

void draw_grid(int spacing, color_t color) {
  for (size_t y = 0; y < window_height; y += spacing) {
    for (size_t x = 0; x < window_width; x += spacing) {
      draw_pixel(x, y, color);
    }
  }
}

void draw_rect(int x, int y, int width, int height, color_t color) {
  for (size_t i = x; i < x + width; i++) {
    for (size_t j = y; j < y + height; j++) {
      draw_pixel(i, j, color);
    }
  }
}
