#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL_image.h>
//player look direction enum
enum PlayerDirection {NONE, RIGHT, LEFT, UP, DOWN};
class Player {
private:
	//for hitboxes
	int x,y;
	int w,h;


	//Rect of the source texture
	SDL_Rect src;
	SDL_Rect dst;
	SDL_Texture *texture;

	//The direction of where the player is looking
	PlayerDirection direction;
	int num_jumps;

	//keypresses
	const Uint8 *keys;

	Uint32 items[13];

	//movement velocity
	int hspeed;
	int vspeed;

public:
	Player();
	Player(int x, int y, SDL_Texture *texture);
	~Player();
	void draw(SDL_Renderer *renderer);
	void input();
	void update();
	void setTexture(SDL_Texture *texture);
	void setY(int num);
	void setX(int num);
	void setXY(int x, int y);
};

#endif // !PLAYER_H
