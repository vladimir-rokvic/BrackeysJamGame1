#include "player.hpp"
#include "item.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <cstring>

#define PLAYER_SPEED 5

Player::Player() {
	this->x = 0;
	this->y = 0;
	this->w = 32;
	this->h = 64;
	this->vspeed = 0;
	this->hspeed = 0;
	this->num_jumps = 1;
	this->direction = NONE;
	memset(this->items, 0, 13*4);
	items[LADY_BUG] = 10;
	keys = SDL_GetKeyboardState(NULL);
	this->texture = NULL;
}

Player::Player(int x, int y, SDL_Texture *texture) {
	keys = SDL_GetKeyboardState(NULL);
	this->x = x;
	this->y = y;
	this->w = 32;
	this->h = 64;
	memset(this->items, 0, 13*4);
	this->direction = NONE;
	this->texture = texture;
	this->vspeed = 0;
	this->hspeed = 0;
	this->num_jumps = 1;
	//for(int i=0;i<13;++i){
	//	printf("%d, ", items[i]);
	//}
}

void Player::draw(SDL_Renderer *renderer) {
	SDL_Rect src = {this->w * this->direction,0,this->w, this->h};
	SDL_Rect dst = {this->x, this->y, this->w*4, this->h*4};
	if(SDL_RenderCopy(renderer, this->texture, &src, &dst) < 0) {
		fprintf(stderr, "Error drawing player: %s", SDL_GetError());
	}
}

void Player::setX(int num){
	this->x = num;
}

void Player::setY(int num){
	this->y = num;
}

void Player::input(){
	this->hspeed = (10 * (keys[SDL_SCANCODE_RIGHT] - keys[SDL_SCANCODE_LEFT]))
	* (1 + 0.1 * this->items[RABBITS_FOOT]);

	this->vspeed = ((num_jumps-- > 0)? 1:0) * (50 * keys[SDL_SCANCODE_SPACE] + 1 * this->items[RABBITS_FOOT]);
	//printf("vspeed: %d", this->vspeed);

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
