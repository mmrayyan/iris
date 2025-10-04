#include "display.h"
#include "draw.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

uint32_t *color_buffer = NULL;
float *z_buffer = NULL;
SDL_Texture *color_buffer_texture = NULL;

const int window_width = 800;
const int window_height = 600;

bool initialize_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }

  window =
      SDL_CreateWindow("Iris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       window_width, window_height, 0);
  if (!window) {
    fprintf(stderr, "Error creating SDL window.\n");
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    fprintf(stderr, "Error creating SDL renderer.\n");
    return false;
  }

  return true;
}

void destroy_window(void) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void render_color_buffer(void) {
  SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer,
                    sizeof(uint32_t) * window_width);
  SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void clear_color_buffer(color_t color) {
  for (size_t y = 0; y < window_height; y++) {
    for (size_t x = 0; x < window_width; x++) {
      draw_pixel(x, y, color);
    }
  }
}

void clear_z_buffer() {
  for (size_t y = 0; y < window_height; y++) {
    for (size_t x = 0; x < window_width; x++) {
      z_buffer[(window_width * y) + x] = 1.0;
    }
  }
}
