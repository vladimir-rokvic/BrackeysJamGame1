#include "entity.hpp"

Entity::Entity(const int x, const int y, const int w, const int h, SDL_Texture *texture){
	this->x = x;
	this->y = y;
	this->w = w*2;
	this->h = h*2;
	this->texture = texture;
	this->src = {0, 0, w, h};
	this->dst = {x, y, w*2, h*2};
}

int Entity::getX() const {
	return x;
}

int Entity::getY() const {
	return y;
}

int Entity::getW() const {
	return w;
}

int Entity::getH() const {
	return h;
}

SDL_Rect* Entity::getDst() {
	return &dst;
}

Walker::Walker(const int x, const int y, const int w, const int h, SDL_Texture *texture):
	Entity(x, y, w, h, texture),
	walk(0, 0, w, h, 2, 60){
	this->frame = 1;
	this->hspeed = 0;
	this->vspeed = 0;
	this->hp = 30;
}

void Walker::process(const int x, const int y) {
	if(x < this->x)
		hspeed = -3;
	else hspeed = 3;
}

void Walker::update() {
	x += hspeed;
}

void Walker::draw(SDL_Renderer *renderer) {
	dst.x = x;
	walk.loop_play(renderer, texture, dst);
}

bool Walker::take_damage(const int dmg) {
	hp -= dmg;
	return hp <= 0;
}
