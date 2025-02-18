#include "animation.hpp"

Animation::Animation(int x, int y, int w, int h, Uint8 num_f, Uint8 d):
	num_frames(num_f), duration(d), frame_duration(d/num_frames){
	this->frame_rect.x = x;
	this->frame_rect.y = y;
	this->frame_rect.w = w;
	this->frame_rect.h = h;
	this->frame = frame_duration;
	this->playing = false;
	this->iter = 0;
}

Uint8 Animation::play(SDL_Renderer *renderer, SDL_Texture *texture,
					  SDL_Rect dst, PlayerDirection direction) {
	if(direction == NONE) direction = LEFT;
	frame++;
	if(frame >= frame_duration){
		playing = true;
		frame_rect.x = iter * frame_rect.w;
		frame_rect.y = frame_rect.h * direction;
		if(++iter == num_frames){
			playing = false;
			iter = 0;
			frame = frame_duration;
		}else frame = 0;
	}
	dst.h = frame_rect.h * 2;
	dst.w = frame_rect.w * 2;
	if(direction == RIGHT)
		SDL_RenderCopy(renderer, texture, &frame_rect, &dst);
	else if(direction == LEFT){
		dst.x -= 96;
		SDL_RenderCopy(renderer, texture, &frame_rect, &dst);
	}

	return playing;
}
