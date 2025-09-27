#ifndef DRAW_H
#define DRAW_H

#include "color.h"
#include "triangle.h"

void draw_pixel(int x, int y, color_t color);
void draw_line(int x0, int y0, int x1, int y1, color_t color);
void draw_grid(int spacing, color_t color);
void draw_rect(int x, int y, int width, int height, color_t color);

#endif
