#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL_image.h>

#include "animation.hpp"

#define FLOOR 840
#define GRAVITY 1
#define PLAYER_SPEED 6

//player look direction enum
enum PlayingAnimation {NO_ANIMATION, ATTACK};
enum PlayerAction {ATTACKING, WALKING, STAR};
class Player {
private:
	//Rect of the source texture
	SDL_Rect src;
	SDL_Texture *texture;
	SDL_Texture *hp_texture;

	int attack_cooldown;
	int num_jumps;
	int max_hp;

	//keypresses
	const Uint8 *keys;
	bool pressed_jump;
	bool pressed_attack;

	Uint32 items[13];

	PlayerAction action;

	PlayingAnimation playing_animation;
	Animation right_attack;
	Animation left_attack;
	Animation up_attack;
	Animation down_attack;

public:
	Player();
	Player(int x, int y, SDL_Texture *texture);
	~Player();
	void draw(SDL_Renderer *renderer);
	void draw_hud(SDL_Renderer *renderer);
	PlayerAction input();
	void update();
	void animate_attack(SDL_Renderer *renderer);
	void setTexture(SDL_Texture *texture);
	void setY(int num);
	void setX(int num);
	void setXY(int x, int y);
	void setHp(int hp);
	void setHpTexture(SDL_Texture *texture);
	int get_damage();
	void heal();
	Uint32 get_star_dmg();
	void restart();
	void recieve_coins();
	void generate_income();
	Uint32* getItems();
	Uint8 hp;
	//for hitboxes
	int x,y;
	int w,h;
	//movement velocity
	int hspeed;
	int vspeed;
	Uint32 coins;
	SDL_Rect dst;
	//The direction of where the player is looking
	PlayerDirection direction;
};

#endif // !PLAYER_H
