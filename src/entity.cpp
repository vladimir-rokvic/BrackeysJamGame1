#include "entity.hpp"

Entity::Entity(const int x, const int y, const int w, const int h, SDL_Texture *texture) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->texture = texture;
	this->src = {0, 0, w, h};
	this->dst = {x, y, w*2, h*2};
}

SDL_Texture* Entity::getTexture() const {
	return this->texture;
}

Entity::~Entity() {
	SDL_DestroyTexture(texture);
}

Walker::Walker(const int x, const int y, const int w, const int h, SDL_Texture *texture):
	Entity(x, y, w, h, texture){
	this->frame = 0;
}

void Walker::move() {

}

void Walker::draw(SDL_Renderer *renderer) {
	frame = !frame;
	SDL_Texture *texture = getTexture();
	src.x += w*frame;
	SDL_RenderCopy(renderer, texture, &this->src, &this->dst);
}
