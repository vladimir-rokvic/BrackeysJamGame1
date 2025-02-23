#include "player.hpp"
#include "animation.hpp"
#include "item.hpp"
#include <SDL2/SDL_stdinc.h>
#include <cstdio>
#include <random>

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
	this->num_jumps = 2;

	this->direction = NONE;
	this->playing_animation = NO_ANIMATION;

	keys = SDL_GetKeyboardState(NULL);
	memset(this->items, 0, 13*4);
	this->coins = 50;

	this->hp = 5;
	this->max_hp = 5;
}

Uint32* Player::getItems(){
	return items;
}

void Player::restart() {
	memset(this->items, 0, 13*4);
	this->coins = 50;
	this->max_hp = 5;
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

void Player::recieve_coins() {
	coins += 10 * (1 + 0.5*items[LUCKY_COIN]);
}

int min(int x, int y){
	return (x<y)? x:y;
}

void Player::draw_hud(SDL_Renderer *renderer) {
	SDL_Rect src = {0, 0, 32, 32};
	SDL_Rect dst = {30, 30, 32, 32};
	int i = 0;
	for(;i<min(5, hp);++i){
		dst.x += 42;
		SDL_RenderCopy(renderer, hp_texture, &src, &dst);
	}
	src.x = 32;
	for(;i<hp;++i){
		dst.x += 42;
		SDL_RenderCopy(renderer, hp_texture, &src, &dst);
	}
	src.x = 64;
	for(;i<max_hp;++i){
		dst.x += 42;
		SDL_RenderCopy(renderer, hp_texture, &src, &dst);
	}
}

void Player::setHpTexture(SDL_Texture *texture){
	this->hp_texture = texture;
}

Uint32 Player::get_star_dmg() {
	return 20*items[FALLING_STAR];
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

void Player::heal() {
	hp = 5 + items[ACORN_HELMET];
	max_hp = hp;
}

void Player::update() {
	if(this->y < FLOOR || vspeed > 0){
		this->y -= this->vspeed;
		this->vspeed -= GRAVITY;
	}else{
		this->y = FLOOR;
		this->num_jumps = 1 + (items[LADY_BUG] >= 1);
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

void Player::generate_income() {
	coins += items[GOLDFISH];
}

std::random_device rd_rd;
std::default_random_engine rd_gen(rd_rd());
std::uniform_int_distribution<> rd_d(1, 6);

int Player::get_damage() {
	Uint32 dmg = 10 + (10*items[WISHBONE]);
	if(y <= FLOOR) dmg += 15*items[LADY_BUG];
	if(items[LUCKY_CAT]) dmg += coins*0.15*items[LUCKY_CAT];
	if(!(rd_d(rd_gen) % (7-items[LUCKY_DIE]))){
		dmg *= (2 + 0.2*items[HORSESHOE]);
	}
	return dmg;
}

Player::~Player() {
	SDL_DestroyTexture(this->texture);
	this->texture = NULL;
}
