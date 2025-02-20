#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL_image.h>

#include "animation.hpp"

#define FLOOR 840
#define GRAVITY 1
#define PLAYER_SPEED 6

//player look direction enum
enum PlayingAnimation {NO_ANIMATION, ATTACK};
enum PlayerAction {ATTACKING, WALKING};
class Player {
private:
	//Rect of the source texture
	SDL_Rect src;
	SDL_Texture *texture;

	int attack_cooldown;
	int num_jumps;

	//keypresses
	const Uint8 *keys;
	bool pressed_jump;
	bool pressed_attack;

	Uint32 items[13];

	//movement velocity
	int hspeed;
	int vspeed;

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
	PlayerAction input();
	void update();
	void animate_attack(SDL_Renderer *renderer);
	void setTexture(SDL_Texture *texture);
	void setY(int num);
	void setX(int num);
	void setXY(int x, int y);
	void setHp(int hp);
	int get_damage();
	Uint8 hp;
	//for hitboxes
	int x,y;
	int w,h;
	SDL_Rect dst;
	//The direction of where the player is looking
	PlayerDirection direction;
};

#endif // !PLAYER_H
