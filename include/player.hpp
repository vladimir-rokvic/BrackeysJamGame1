#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include "item.hpp"
//player look direction enum
enum PlayerDirection {NONE, RIGHT, LEFT, UP, DOWN};
class Player {
private:
	int x,y;
	int w,h;
	SDL_Texture *texture;
	PlayerDirection direction;
	const Uint8 *keys;
	Uint32 items[13];

public:
	Player();
	Player(int x, int y, SDL_Texture *texture);
	~Player();
	void draw(SDL_Renderer *renderer);
	void input();
	void setTexture(SDL_Texture *texture);
	void setXY(int x, int y);
};

#endif // !PLAYER_H
