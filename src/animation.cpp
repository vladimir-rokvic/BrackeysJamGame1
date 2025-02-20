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
					  SDL_Rect dst, const int x_offset, const int y_offset) {
	++frame;
	if(frame >= frame_duration){
		playing = true;
		frame_rect.x = iter * frame_rect.w;
		if(++iter == num_frames){
			playing = false;
			iter = 0;
			frame = frame_duration;
		}else frame = 0;
	}
	dst.h = frame_rect.h * 2;
	dst.w = frame_rect.w * 2;
	dst.x += x_offset;
	dst.y += y_offset;
	SDL_RenderCopy(renderer, texture, &frame_rect, &dst);

	return playing;
}

void Animation::loop_play(SDL_Renderer *renderer, SDL_Texture *texture,SDL_Rect dst){
	++frame;
	frame = (frame >= duration)? 0:frame;
	frame_rect.x = frame/frame_duration * frame_rect.w;
	dst.h = frame_rect.h * 2;
	dst.w = frame_rect.w * 2;
	SDL_RenderCopy(renderer, texture, &frame_rect, &dst);
}
