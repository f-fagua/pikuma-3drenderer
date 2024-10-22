#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

bool is_running = false;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

// Declare a pointer to an array of int32 elements
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

void setup(void) 
{
	// Allocate the required bytes in memory for the color buffer
	color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
	
	// Creating a SDL texture that is used to display the color buffer
	color_buffer_texture = SDL_CreateTexture
	(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);
}

void process_input(void)
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
		case SDL_QUIT:
		{
			is_running = false;
			break;
		}
		case SDL_KEYDOWN:
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				is_running = false;
			}
			break;
		}
	}
}

void update(void)
{
	// TODO:
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

void draw_rect(int rect_x, int rect_y, int width, int height, uint32_t rect_color) 
{
	for (int x = rect_x; x < rect_x + width; x++) 
	{
		for (int y = rect_y; y < rect_y + height; y++) 
		{
			if (x > 0 && x < window_width && y > 0 && y < window_height)
			{
				color_buffer[(window_width * y) + x] = rect_color;
			}
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

void render(void)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);

	draw_grid(10, 0xFF00FF00);
	draw_rect(1800, 150, 400, 300, 0xFFF699CD);

	render_color_buffer();
	clear_color_buffer(0xFF000000);

	SDL_RenderPresent(renderer);
}


void destroy_window(void) 
{
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


int main(void)
{
	is_running = initialize_window();

	setup();

	while(is_running) 
	{
		process_input();
		update();
		render();
	}

	destroy_window();

	return 0;	
}