#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL2/SDL.h>

enum PlayerDirection {NONE, RIGHT, LEFT, UP, DOWN};
class Animation {
private:
	//number on frames of the animation
	const Uint8 num_frames;
	//the width and height of the frames
	SDL_Rect frame_rect;
	//how many frames the animation should last;
	const int duration;
	//duration of one frame
	const Uint8 frame_duration;

	//Current frame of the game
	Uint8 frame;
	Uint8 iter;

public:
	bool playing;
	Animation(int x, int y, int w, int h, Uint8 num_f, Uint8 d);
	Uint8 play(SDL_Renderer *renderer,SDL_Texture *texture,
			SDL_Rect dst, PlayerDirection direction);
	void loop_play(SDL_Renderer *renderer);
};

#endif // !ANIMATION_H
