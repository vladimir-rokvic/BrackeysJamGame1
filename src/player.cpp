#include "player.hpp"
#include "item.hpp"
#include <SDL2/SDL_render.h>
#include <cstring>

#define PLAYER_SPEED 5

Player::Player() {
	this->x = 0;
	this->y = 0;
	this->w = 32;
	this->h = 64;
	memset(this->items, 0, 13*4);
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

void Player::input(){
	double vspeed = 0;
	double hspeed = (5 * (keys[SDL_SCANCODE_RIGHT] - keys[SDL_SCANCODE_LEFT]))
	* (1 + 0.1 * this->items[RABBITS_FOOT]);
	if(hspeed < 0)
		this->direction = LEFT;
	else if(hspeed > 0)
		this->direction = RIGHT;
	else this->direction = NONE;

	if(keys[SDL_SCANCODE_UP])
		this->direction = UP;
	else if(keys[SDL_SCANCODE_DOWN])
		this->direction = DOWN;
	
	this->x += hspeed;
	this->y += vspeed;
}

Player::~Player() {
	SDL_DestroyTexture(this->texture);
}

void Player::setTexture(SDL_Texture *texture) {
	this->texture = texture;
}

void Player::setXY(int x, int y) {
	this->x = x;
	this->y = y;
}
