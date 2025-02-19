#include "player.hpp"
#include "animation.hpp"
#include "item.hpp"

Player::Player():
	//x,y,w,h,num_frames,duration
	attack(0, 64, 80, 64, 4, 16){
	this->src = {
		.x = 0,
		.y = 0,
		.w = 32,
		.h = 64
	};

	this->dst = {
		.x = 0,
		.y = 0,
		.w = 64,
		.h = 128
	};

	this->texture = NULL;

	//for hitboxes
	this->x = 0;
	this->y = 0;
	this->w = 64;
	this->h = 64;

	this->vspeed = 0;
	this->hspeed = 0;

	this->pressed_attack = false;
	this->pressed_jump = false;
	this->num_jumps = 1;

	this->direction = NONE;
	this->playing_animation = NO_ANIMATION;
	this->action = STANDING;

	keys = SDL_GetKeyboardState(NULL);
	memset(this->items, 0, 13*4);
}

void Player::draw(SDL_Renderer *renderer) {
	dst.x = x;
	dst.y = y;
	src.x = src.w * direction;
	switch(playing_animation){
		case ATTACK:{
			if(!attack.play(renderer, texture, dst, direction))
				playing_animation = NO_ANIMATION;
		}break;
		default:{
			if(SDL_RenderCopy(renderer, this->texture, &src, &dst) < 0) {
				fprintf(stderr, "Error drawing player: %s", SDL_GetError());
			}
		}break;
	}
}

#define PLAYER_SPEED 8
void Player::input() {
	if(playing_animation  == NO_ANIMATION && pressed_attack ^ keys[SDL_SCANCODE_X]){
		if(keys[SDL_SCANCODE_X])
			playing_animation = ATTACK;
	}
	pressed_attack = keys[SDL_SCANCODE_X];

	const int8_t dir = (keys[SDL_SCANCODE_RIGHT] - keys[SDL_SCANCODE_LEFT]);
	this->hspeed = (PLAYER_SPEED * dir)
	* (1 + 0.1 * this->items[RABBITS_FOOT]);

	if(num_jumps && pressed_jump ^ keys[SDL_SCANCODE_SPACE]){
		this->vspeed = 45 * keys[SDL_SCANCODE_SPACE];
		num_jumps--;
	}
	pressed_jump = keys[SDL_SCANCODE_SPACE];

	if(this->hspeed < 0)
		this->direction = LEFT;
	else if(this->hspeed > 0)
		this->direction = RIGHT;
	else this->direction = NONE;

	if(keys[SDL_SCANCODE_UP])
		this->direction = UP;
	else if(keys[SDL_SCANCODE_DOWN])
		this->direction = DOWN;

}

#define FLOOR 840
#define GRAVITY 3

void Player::update() {
	if(this->y < FLOOR || vspeed > 0){
		this->y -= this->vspeed;
		this->vspeed -= GRAVITY;
	}else{
		this->y = FLOOR;
		this->num_jumps = 1 + this->items[LADY_BUG];
	}

	this->x += hspeed;

	if(x < 0 || x + 56 > 1920)
		x -= hspeed;
}

void Player::setX(int num){
	this->x = num;
}

void Player::setY(int num){
	this->y = num;
}
void Player::setTexture(SDL_Texture *texture) {
	this->texture = texture;
}

void Player::setXY(int x, int y) {
	this->x = x;
	this->y = y;
}

Player::~Player() {
	SDL_DestroyTexture(this->texture);
	this->texture = NULL;
}
