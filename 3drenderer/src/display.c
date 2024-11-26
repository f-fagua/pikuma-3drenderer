#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

static uint32_t* color_buffer = NULL;
static float* z_buffer = NULL;

static SDL_Texture* color_buffer_texture = NULL;
static int window_width = 640;
static int window_height = 400;

static int render_method = 0;
static int cull_method = 0;

int get_window_width(void) 
{
	return window_width;
}

int get_window_height(void) 
{
	return window_height;
}

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
	int fullscreen_width = display_mode.w;
	int fullscreen_height = display_mode.h;

	window_width = fullscreen_width / 3;
	window_height = fullscreen_height / 3;

	// Create a SDL Window
	window = SDL_CreateWindow(
		NULL, 
		SDL_WINDOWPOS_CENTERED,  	// initial x
		SDL_WINDOWPOS_CENTERED,		// initial y
		window_width,				// width
		window_height,				// height
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

	// Allocate the required bytes in memory for the color buffer
	color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
	z_buffer = (float*) malloc(sizeof(float) * window_width * window_height);

	// Creating a SDL texture that is used to display the color buffer
	color_buffer_texture = SDL_CreateTexture
	(
		renderer,
		SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);

	return true;
}

void set_render_method(int method)
{
	render_method = method;
}

void set_cull_method(int method)
{
	cull_method = method;
}

bool is_cull_backface(void)
{
	return cull_method == CULL_BACKFACE;
}

bool should_render_filled_triangle(void)
{
	return 
	(
		render_method == RENDER_FILL_TRIANGLE || 
		render_method == RENDER_FILL_TRIANGLE_WIRE
	);
}

bool should_render_textured_triangle(void)
{
	return 
	(
		render_method == RENDER_TEXTURED || 
		render_method == RENDER_TEXTURED_WIRE
	);	
}

bool should_render_unfiled_triangle(void)
{
	return 
	(
		render_method == RENDER_WIRE || 
		render_method == RENDER_WIRE_VERTEX || 
		render_method == RENDER_FILL_TRIANGLE_WIRE || 
		render_method == RENDER_TEXTURED_WIRE
	);	
}

bool should_render_wired_vertex(void)
{
	return render_method == RENDER_WIRE_VERTEX;	
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
	if (x < 0 || x >= window_width || y < 0 || y >= window_height)
	{
		return;
	}
	color_buffer[(window_width * y) + x] = color;	
}

void draw_line(int x0, int y0, int x1, int y1, uint32_t color) 
{
	int delta_x = (x1 - x0); // run
	int delta_y = (y1 - y0); // rise

	int longest_side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y); // We need to pick the largest to do the increment by 1

	// Find how much we should increment in both x and y each step
	float x_inc = delta_x / (float)longest_side_length; // (depending the longest) 1, -1, or 0 
	float y_inc = delta_y / (float)longest_side_length; // (depending the longest) not always 1, it depends on the slope

	float current_x = x0;
	float current_y = y0;

	for (int i = 0; i <= longest_side_length; i++) 
	{
		draw_pixel(round(current_x), round(current_y), color);
		current_x += x_inc;
		current_y += y_inc;
	}
}

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	draw_line(x0, y0, x1, y1, color);
	draw_line(x1, y1, x2, y2, color);
	draw_line(x2, y2, x0, y0, color);
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
	SDL_RenderPresent(renderer);
}

void clear_color_buffer(uint32_t color)
{
	for (int i = 0; i < window_width * window_height; i++) 
	{
		color_buffer[i] = color;
	}
}

void clear_z_buffer()
{
	for (int i = 0; i < window_width * window_height; i++) 
	{
		z_buffer[i] = 1.0;
	}
}

float get_zbuffer_at(int x, int y)
{
	if (x < 0 || x >= window_width || y < 0 || y >= window_height)
	{
		return 1.0;
	}
	return z_buffer[(window_width * y) + x];
}

void update_zbuffer_at(int x, int y, float value)
{
	if (x < 0 || x >= window_width || y < 0 || y >= window_height)
	{
		return;
	}

	z_buffer[(window_width * y) + x] = value;
}

void destroy_window(void) 
{
	free(color_buffer);
	free(z_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}