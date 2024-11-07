#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;
int window_width = 800;
int window_height = 600;

bool initialize_window(void) 
{
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}

	// Use SDL to query what is the fullscreen max. width & height
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	window_width = display_mode.w;
	window_height = display_mode.h;

	// Create a SDL Window
	window = SDL_CreateWindow(
		NULL, 
		SDL_WINDOWPOS_CENTERED,  	// initial x
		SDL_WINDOWPOS_CENTERED,		// initial y
		window_width,						// width
		window_height,						// height
		SDL_WINDOW_BORDERLESS		// ui option
									// more falgs
	);

	if (!window)
	{
		fprintf(stderr, "Error creating SDL window.\n");
	}

	// Create a SDL renderer
	renderer = SDL_CreateRenderer(
		window, // Pointer to the window.
		-1, 	// Default display device (-1 is the first default one).
		0		// More falgs
	);
	
	if (!renderer) 
	{
		fprintf(stderr, "Error creating SDL renderer");
		return false;
	}

	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	return true;
}

void draw_grid(int grid_size, uint32_t grid_color)
{
	for (int y = 0; y < window_height; y += grid_size) 
	{
		for (int x = 0; x < window_width; x += grid_size)
		{
			color_buffer[(window_width * y) + x] = grid_color;
		}
	}
}

void draw_pixel(int x, int y, uint32_t color)
{
	if (x >= 0 && x < window_width && y >= 0 && y < window_height)
	{
		color_buffer[(window_width * y) + x] = color;	
	}
}

void draw_line(int x0, int y0, int x1, int y1, uint32_t color) 
{
	int delta_x = (x1 - x0); // run
	int delta_y = (y1 - y0); // rise

	int side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y); // We need to pick the largest to do the increment by 1

	// Find how much we should increment in both x and y each step
	float x_inc = delta_x / (float)side_length; // (depending the greater) 1, -1, or 0 
	float y_inc = delta_y / (float)side_length; // (depending the greater) not always 1, it depends on the slope

	float current_x = x0;
	float current_y = y0;

	for (int i = 0; i <= side_length; i++) 
	{
		draw_pixel(round(current_x), round(current_y), color);
		current_x += x_inc;
		current_y += y_inc;
	}
}

void draw_rect(int rect_x, int rect_y, int width, int height, uint32_t rect_color) 
{
	for (int x = rect_x; x < rect_x + width; x++) 
	{
		for (int y = rect_y; y < rect_y + height; y++) 
		{
			draw_pixel(x, y, rect_color);
		}	
	}
}

void render_color_buffer(void) 
{
	SDL_UpdateTexture(
		color_buffer_texture,
		NULL,
		color_buffer,
		(int)(window_width * sizeof(uint32_t)) // pitch
	);

	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void clear_color_buffer(uint32_t color)
{
	for (int y = 0; y < window_height; y++) 
	{
		for (int x = 0; x < window_width; x++) 
		{
			color_buffer[(window_width * y) + x] = color;
		}
	}

	// Set the pixel at row 10 column 20 to the color red
	// color_buffer[(window_width * 10) + 20] = 0xFFFF0000;
}

void destroy_window(void) 
{
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}