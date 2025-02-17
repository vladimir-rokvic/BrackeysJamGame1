#include "game.hpp"
#include <SDL2/SDL_events.h>

int main(int argc, char* argv[]){
	Game game;
	SDL_Event ev;
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
	}

	return 0;
}
