#include "player.hpp"
#include "item.hpp"


Player::Player() {
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

	this->num_jumps = 1;
	this->direction = NONE;

	keys = SDL_GetKeyboardState(NULL);
	memset(this->items, 0, 13*4);
}

void Player::draw(SDL_Renderer *renderer) {
	dst.x = x;
	dst.y = y;
	src.x = src.w * direction;
	if(SDL_RenderCopy(renderer, this->texture, &src, &dst) < 0) {
		fprintf(stderr, "Error drawing player: %s", SDL_GetError());
	}
}

#define PLAYER_SPEED 8
void Player::input() {
	const int8_t dir = (keys[SDL_SCANCODE_RIGHT] - keys[SDL_SCANCODE_LEFT]);
	this->hspeed = (PLAYER_SPEED * dir)
	* (1 + 0.1 * this->items[RABBITS_FOOT]);

	if(num_jumps){
		this->vspeed = (50 * keys[SDL_SCANCODE_SPACE] + 1 * this->items[RABBITS_FOOT]);
		num_jumps--;
	}

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

void Player::update() {
	if(this->y < FLOOR || vspeed > 0){
		this->y -= this->vspeed;
		this->vspeed -= 5;
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
