#include "game.hpp"
#include <SDL2/SDL_events.h>

int main(int argc, char* argv[]){
	Game game;
	SDL_Event ev;
	Uint32 end;
	Uint32 start = SDL_GetTicks();
	int frame = 0;
	while(game.run){
		while(SDL_PollEvent(&ev))
			switch (ev.type) {
				case SDL_QUIT:{
					game.run = false;
				}break;
				case SDL_KEYDOWN:{
					if(ev.key.repeat == 0)
						game.process();
				}break;
				case SDL_KEYUP:{
					game.process();
				}break;
			}
		game.loop();
		game.render();
		++frame;
		end = SDL_GetTicks();
		if(end - start >= 1000){
			printf("FPS: %d\n", frame);
			frame = 0;
			start = SDL_GetTicks();
		}
	}

	return 0;
}
