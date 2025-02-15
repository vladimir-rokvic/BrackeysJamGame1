#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define ERROR(string) fprintf(stderr, "%s, %s\n", string, SDL_GetError());

void process_event(){
}

int main(int argc, char* argv[]){
	if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0){
		ERROR("Error init");
		exit(-1);
	}

	SDL_Window *window = SDL_CreateWindow("break", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
									   800, 600, SDL_WINDOW_FULLSCREEN);
	if(!window){
		ERROR("Error creating window");
		exit(-1);
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	if(!renderer){
		ERROR("Error creating renderer");
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

	SDL_Quit();
	return 0;
}
