#ifndef DISPLAY_H
#define DISPLAY_H

#include "color.h"
#include "triangle.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

#define FPS 30
#define FRAME_TARGET_TIME 1000 / FPS

enum cull_method {
  CULL_NONE,
  CULL_BACKFACE
};

enum render_method {
  RENDER_WIRE,
  RENDER_WIRE_VERTEX,
  RENDER_FILL_TRIANGLE,
  RENDER_FILL_TRIANGLE_WIRE,
  RENDER_TEXTURED,
  RENDER_TEXTURED_WIRE
};

extern enum cull_method cull_method;
extern enum render_method render_method;

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern uint32_t *color_buffer;
extern SDL_Texture *color_buffer_texture;
extern const int window_width;
extern const int window_height;

bool initialize_window(void);
void destroy_window(void);

void render_color_buffer(void);
void clear_color_buffer(color_t color);

#endif
