#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "player.hpp"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define ERROR(string, error_type) fprintf(stderr, "%s, %s\n", string, error_type)

enum GameScreen {MAIN_MENU, PAUSED, RUNNNING, SCREEN_OPTIONS};
enum MenuOption {START, OPTIONS, QUIT};
enum PausedOption {RESUME, MAIN};

class Game{
private:
	SDL_Window *window;
	SDL_Renderer *renderer;
	Player player;

	GameScreen screen;
	MenuOption menu;
	PausedOption paused;

	void draw_menu();
	void menu_input();

	const Uint8 *keys;

public:
	bool run;
	Game();
	~Game();
	void process();
	void loop();
	void render();
};

#endif // !GAME_H
