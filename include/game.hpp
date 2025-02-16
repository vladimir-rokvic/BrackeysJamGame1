#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "player.hpp"

#define ERROR(string, error_type) fprintf(stderr, "%s, %s\n", string, error_type)

class Game{
private:
	SDL_Window *window;
	SDL_Renderer *renderer;
	Player player;

public:
	Game();
	~Game();
	void process();
	void loop();
	void render();
};

#endif // !GAME_H
