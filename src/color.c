#include "color.h"

color_t light_apply_intensity(color_t original_color, float percentage_factor) {
  if (percentage_factor < 0) {
    percentage_factor = 0;
  }

  color_t a = (original_color & 0xFF000000);
  color_t r = (original_color & 0x00FF0000) * percentage_factor;
  color_t g = (original_color & 0x0000FF00) * percentage_factor;
  color_t b = (original_color & 0x000000FF) * percentage_factor;

  return a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
}
