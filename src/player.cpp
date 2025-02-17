#include "player.hpp"
#include "item.hpp"

#define PLAYER_SPEED 10

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
	this->x = 32;
	this->y = 32;
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
	dst.x = x - 32;
	dst.y = y - 32;
	src.x = src.w * direction;
	if(SDL_RenderCopy(renderer, this->texture, &src, &dst) < 0) {
		fprintf(stderr, "Error drawing player: %s", SDL_GetError());
	}
}

void Player::input() {
	this->hspeed = (PLAYER_SPEED * (keys[SDL_SCANCODE_RIGHT] - keys[SDL_SCANCODE_LEFT]))
	* (1 + 0.1 * this->items[RABBITS_FOOT]);

	this->vspeed = ((num_jumps-- > 0)? 1:0) * (50 * keys[SDL_SCANCODE_SPACE] + 1 * this->items[RABBITS_FOOT]);

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

void Player::update() {
	if(this->y > 0 || vspeed > 0){
		this->y += this->vspeed;
		this->vspeed -= 10;
	}else{
		this->y = 0;
		this->num_jumps = 1 + this->items[LADY_BUG];
	}

	this->x += hspeed;
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
