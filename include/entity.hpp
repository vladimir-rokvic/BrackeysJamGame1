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
	int hspeed, vspeed;

	//Rects to give to SDL_RenderCopy
	SDL_Rect src, dst;
	//Texture spreadsheet of the entity
	SDL_Texture *texture;

public:
	//The players x and y coordinates
	virtual void process(const int x, const int y) {};
	virtual void update() {};
	virtual void draw(SDL_Renderer *renderer) {};
	int getX() const;
	int getY() const;
	int getW() const;
	int getH() const;
	Entity(const int x, const int y, const int w, const int h, SDL_Texture *texture);
	virtual ~Entity();
};

class Walker: public Entity {
	Animation walk;
	Uint8 hp;
	int goal_x;
	//walking frame
	Uint8 frame;
public:
	Walker(int x, int y, int w, int h, SDL_Texture* texture);
	void update();
	void draw(SDL_Renderer *renderer);
	void process(const int x, const int y);
};

#endif // !ENTITY_H
