#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <vector>

#include "player.hpp"
#include "entity.hpp"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define ERROR(string, error_type) fprintf(stderr, "%s, %s\n", string, error_type)

enum GameScreen {MAIN_MENU, PAUSED, RUNNNING, SCREEN_OPTIONS};
enum MenuOption {START, OPTIONS, QUIT};

typedef unsigned long ul;

class Game{
private:
	SDL_Window *window;
	SDL_Renderer *renderer;
	Player player;

	int screen_width;
	int screen_height;
	GameScreen screen;
	MenuOption option;

	const Uint8 *keys;

	SDL_Texture *player_texture;
	SDL_Texture *start_texture;
	SDL_Texture *options_texture;
	SDL_Texture *quit_texture;
	SDL_Texture *chosen;
	SDL_Texture *resume_texture;
	SDL_Texture *main_menu_texture;
	SDL_Texture *background_texture;
	SDL_Texture *walker_texture;

	void draw_menu();
	void change_chosen(SDL_Texture *texture);
	void menu_input();
	bool check_colision(const SDL_Rect *, const SDL_Rect *);
	Entity* spawn_entity();
	void restart();

	std::vector<Entity *> entities;
	Uint32 frame;
	Uint32 player_hit_cooldown;
	PlayerAction player_action;
public:
	bool run;
	Game();
	~Game();
	void process();
	void loop();
	void render();
};

#endif // !GAME_H
