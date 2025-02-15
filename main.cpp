#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>

#define ERROR(string) fprintf(stderr, "%s, %s\n", string, SDL_GetError());

int main() {
	if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0)
		ERROR("Error init");

	SDL_Window *window = SDL_CreateWindow("break", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
									   800, 600, SDL_WINDOW_FULLSCREEN);
	if(!window)
		ERROR("Error creating window");

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	if(!renderer)
		ERROR("Error creating renderer");

	SDL_SetRenderDrawColor(renderer, 0xFF,0x00,0xFF,0xFF);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	SDL_Delay(5000);

	SDL_Quit();
	return 0;
}
