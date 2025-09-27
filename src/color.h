#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

typedef uint32_t color_t;

color_t light_apply_intensity(color_t original_color, float percentage_factor);

#endif
