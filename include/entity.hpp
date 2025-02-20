/*I decided to put all the enemies in the entity file becasue I am lazy
 * and stupid enough to not be able to think up a file structure that would
 * satisfy me
 * */
#ifndef ENTITY_H
#define ENTITY_H

#include "animation.hpp"
#include <SDL2/SDL.h>

enum Entities {WALKER,BALL};

class Entity {
protected:
	//Entity positions in real coordinates
	int x, y;
	int w, h;

	//Rects to give to SDL_RenderCopy
	SDL_Rect src, dst;
	//Texture spreadsheet of the entity
	SDL_Texture *texture;

public:
	virtual void move();
	virtual void draw(SDL_Renderer *renderer);
	Entity(int x, int y, int w, int h, SDL_Texture *texture);
	~Entity();
	SDL_Texture* getTexture() const;
};

class Walker: Entity {
	//Animation walk;
	Walker(int x, int y, int w, int h, SDL_Texture* texture);
	//walking frame
	Uint8 frame;
public:
	void move();
	void draw(SDL_Renderer *renderer);
};

#endif // !ENTITY_H
