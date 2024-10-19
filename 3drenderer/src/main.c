#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

bool is_running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

bool initialize_window(void) 
{
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}

	// Create a SDL Window
	window = SDL_CreateWindow(
		NULL, 
		SDL_WINDOWPOS_CENTERED,  	// initial x
		SDL_WINDOWPOS_CENTERED,		// initial y
		800,						// width
		600,						// height
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
	}

	return true;
}

void setup(void) 
{
	// TODO:
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

void render(void)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);

	//...

	SDL_RenderPresent(renderer);
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


	return 0;	
}