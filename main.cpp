#include "game.hpp"

int main(int argc, char* argv[]){
	Game game;
	bool run = true;
	SDL_Event ev;
	while(run){
		while(SDL_PollEvent(&ev))
			switch (ev.type) {
				case SDL_QUIT:{
					run = false;
				}break;
				default:{
					game.process();
				}
			}
		game.loop();
		game.render();
	}

	return 0;
}
