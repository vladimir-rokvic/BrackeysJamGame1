#include "player.hpp"
#include "animation.hpp"
#include "item.hpp"

Player::Player():
	//x,y,w,h,num_frames,duration
	right_attack(0, 64, 80, 64, 3,  15),
	left_attack(0, 128, 80, 64, 3 , 15),
	up_attack(0, 192, 64, 80, 4, 16),
	down_attack(0, 272, 64, 80, 4, 16){
	this->src = {
		.x = 0,
		.y = 0,
		.w = 32,
		.h = 64
	};

	this->dst = {
		.x = 0,
		.y = 0,
		.w = 64,
		.h = 128
	};

	this->texture = NULL;

	//for hitboxes
	this->x = 0;
	this->y = 0;
	this->w = 64;
	this->h = 64;

	this->vspeed = 0;
	this->hspeed = 0;

	this->attack_cooldown = 0;
	this->pressed_attack = false;
	this->pressed_jump = false;
	this->num_jumps = 1;

	this->direction = NONE;
	this->playing_animation = NO_ANIMATION;

	keys = SDL_GetKeyboardState(NULL);
	memset(this->items, 0, 13*4);
	this->coins = 0;

	this->hp = 5;
}

void Player::draw(SDL_Renderer *renderer) {
	dst.x = x;
	dst.y = y;
	src.x = src.w * direction;
	switch(playing_animation){
		case ATTACK:{
			switch(direction){
				case RIGHT:{
					playing_animation = 
						PlayingAnimation(
							ATTACK * right_attack.play(renderer, texture, dst, 0, 0)
						);
				}break;
				case UP:{
					playing_animation = 
						PlayingAnimation(
							ATTACK * up_attack.play(renderer, texture, dst, -35, -32)
						);
				}break;
				case DOWN:{
					playing_animation = 
						PlayingAnimation(
							ATTACK * down_attack.play(renderer, texture, dst, -35, 78)
						);
				}break;
				default:{
					playing_animation = 
						PlayingAnimation(
							ATTACK * left_attack.play(renderer, texture, dst, -96, 0)
						);
				}break;
			}
		}break;
		default:{
			if(SDL_RenderCopy(renderer, this->texture, &src, &dst) < 0) {
				fprintf(stderr, "Error drawing player: %s", SDL_GetError());
			}
		}break;
	}
}

PlayerAction Player::input() {
	if(playing_animation  == NO_ANIMATION && pressed_attack ^ keys[SDL_SCANCODE_A] &&
		attack_cooldown == 0){
			playing_animation = PlayingAnimation(
			ATTACK * keys[SDL_SCANCODE_A]
		);
		attack_cooldown = 16;
	}
	pressed_attack = keys[SDL_SCANCODE_A];

	if(attack_cooldown == 16) return ATTACKING;

	const int8_t dir = (keys[SDL_SCANCODE_RIGHT] - keys[SDL_SCANCODE_LEFT]);
	this->hspeed = (PLAYER_SPEED * dir)
	* (1 + 0.1 * this->items[RABBITS_FOOT]);

	if(num_jumps && pressed_jump ^ keys[SDL_SCANCODE_SPACE]){
		this->vspeed = 25 * keys[SDL_SCANCODE_SPACE];
		num_jumps--;
	}
	pressed_jump = keys[SDL_SCANCODE_SPACE];

	if(this->hspeed < 0)
		this->direction = LEFT;
	else if(this->hspeed > 0)
		this->direction = RIGHT;

	if(keys[SDL_SCANCODE_UP])
		this->direction = UP;
	else if(keys[SDL_SCANCODE_DOWN])
		this->direction = DOWN;

	return WALKING;
}

void Player::update() {
	if(this->y < FLOOR || vspeed > 0){
		this->y -= this->vspeed;
		this->vspeed -= GRAVITY;
	}else{
		this->y = FLOOR;
		this->num_jumps = 1 + this->items[LADY_BUG];
	}

	this->x += hspeed;
	if(x < 0 || x + 56 > 1920)
		x -= hspeed;

	if(attack_cooldown) --attack_cooldown;
}

void Player::setX(int num){
	this->x = num;
}

void Player::setY(int num){
	this->y = num;
}
void Player::setTexture(SDL_Texture *texture) {
	this->texture = texture;
}

void Player::setXY(int x, int y) {
	this->x = x;
	this->y = y;
}

void Player::setHp(int hp) {
	this->hp = hp;
}

int Player::get_damage() {
	//10 is the base damage of the weapon
	return 10;
}

Player::~Player() {
	SDL_DestroyTexture(this->texture);
	this->texture = NULL;
}
