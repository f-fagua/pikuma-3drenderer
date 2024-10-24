#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

extern SDL_Window* window;
extern SDL_Renderer* renderer;

// Declare a pointer to an array of int32 elements
extern uint32_t* color_buffer;
extern SDL_Texture* color_buffer_texture;

extern int window_width;
extern int window_height;

bool initialize_window(void);
void draw_grid(int grid_size, uint32_t grid_color);
void draw_pixel(int x, int y, uint32_t color);
void draw_rect(int rect_x, int rect_y, int width, int height, uint32_t rect_color);
void render_color_buffer(void);
void clear_color_buffer(uint32_t color);
void destroy_window(void);

#endif