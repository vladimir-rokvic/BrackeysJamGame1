#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_render.h>
#include <vector>

#include "player.hpp"
#include "entity.hpp"
#include "item.hpp"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define ERROR(string, error_type) fprintf(stderr, "%s, %s\n", string, error_type)

enum GameScreen {MAIN_MENU, PAUSED, RUNNING, RUNNING_SHOP, SCREEN_OPTIONS};
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

	//just there for menu
	SDL_Texture *chosen;

	SDL_Texture *player_texture;
	SDL_Texture *start_texture;
	SDL_Texture *options_texture;
	SDL_Texture *quit_texture;
	SDL_Texture *resume_texture;
	SDL_Texture *main_menu_texture;
	SDL_Texture *background_texture;
	SDL_Texture *walker_texture;
	SDL_Texture *shop_texture;
	SDL_Texture *shop_row_texture;
	SDL_Texture *text_texture;
	SDL_Texture *items_texture;
	SDL_Texture *altar_texture;
	SDL_Texture *hp_texture;
	SDL_Texture *c_texture;

	int star_cooldown;
	Uint32 iter;
	int enemy_spawn_hp;

	SDL_Rect altar_dst;

	void draw_shop();
	void draw_menu();
	void change_chosen(SDL_Texture *texture);
	void menu_input();
	bool check_colision(const SDL_Rect *, const SDL_Rect *);
	void spawn_entity();
	void restart();
	void render_text(std::string, int x, int y);
	void generate_shop();
	void win();
	void draw_altar();

	int num_tickets;

	std::vector<Entity *> entities;
	Uint32 frame;
	Uint32 player_hit_cooldown;
	PlayerAction player_action;

	Item max_item[13];
	Item *item_shop[4];

	int seconds;

	Uint8 shop_option;

	Uint32 max_enemies;
	Uint32 number_spawned;
public:
	bool run;
	Game();
	~Game();
	void process();
	void loop();
	void render();
};

#endif // !GAME_H
