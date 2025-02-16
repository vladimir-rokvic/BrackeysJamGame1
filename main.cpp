#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#define ERROR(string, error_type) fprintf(stderr, "%s, %s\n", string, error_type)

void process_event(){
}

int main(int argc, char* argv[]){
	if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0){
		ERROR("Error init", SDL_GetError());
		exit(-1);
	}

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	SDL_Window *window = SDL_CreateWindow("break", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
									   800, 600, SDL_WINDOW_FULLSCREEN);
	if(!window){
		ERROR("Error creating window", SDL_GetError());
		exit(-1);
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(!renderer){
		ERROR("Error creating renderer", SDL_GetError());
		exit(-1);
	}

	SDL_RenderPresent(renderer);

	bool run = true;
	SDL_Event ev;
	while(run){
		while(SDL_PollEvent(&ev))
			switch (ev.type) {
				case SDL_QUIT:{
					run = false;
				}break;
				default:{
					process_event();
				}break;
			}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
