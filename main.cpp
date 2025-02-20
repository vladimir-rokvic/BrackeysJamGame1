#include "game.hpp"
#include <SDL2/SDL_events.h>

#define FRAME_DURATION 1000/60

int main(int argc, char* argv[]){
	Game game;
	SDL_Event ev;
	//For counting the fps
	//Uint32 end, start = SDL_GetTicks();
	//For capping the framerate to ~60
	Uint32 frame_start, frame_end;
	Uint32 frame = 0;
	while(game.run){
		frame_start = SDL_GetTicks();

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

		frame_end = SDL_GetTicks();
		if(frame_end - frame_start < FRAME_DURATION){
			SDL_Delay(FRAME_DURATION - (frame_end - frame_start));
		}
		//end = SDL_GetTicks();
		//if(end - start >= 1000){
		//	printf("FPS: %d\n", frame);
		//	frame = 0;
		//	start = SDL_GetTicks();
		//}
	}

	return 0;
}
