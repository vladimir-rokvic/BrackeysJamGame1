/*I decided to put all the enemies in the entity file becasue I am lazy
 * and stupid enough to not be able to think up a file structure that would
 * satisfy me
 * */
#ifndef ENTITY_H
#define ENTITY_H

#include "animation.hpp"
#include <SDL2/SDL.h>

class Entity {
private:
	//Entity positions in real coordinates
	int x, y;
	int w, h;

	//Rects to give to SDL_RenderCopy
	SDL_Rect src, dst;
	//Texture spreadsheet of the entity
	SDL_Texture *texture;

public:
	virtual void move();
};

class Walker:Entity {
	Animation walk;
	Walker();
public:
	void move();
};

#endif // !ENTITY_H
