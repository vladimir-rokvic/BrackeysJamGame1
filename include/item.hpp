#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <string.h>
#include <SDL2/SDL.h>
//TODO: rework as nessecary
enum Tag {
	LUCKY_DIE, //Increases crit chance up to 99 then it increases crit damage 
	DREAM_CATCHER, //Returns damage to the enemy (can crit)
	RABBITS_FOOT, //Increases speed and dodge chance 
	LADY_BUG, //Double jump then it increases downward damage
	ACORN_HELMET, //Increases armor 
	LUCKY_COIN, //Money chances up
	HORSESHOE, //Ranged weapon(boomerang style)
	EVIL_EYE, //Predict the stock market (discount)
	LUCKY_CAT, //Damage based on your money 
	WISHBONE, //Teleport 
	GOLDFISH, //Reduces cooldowns 
	WINNING_TICKET,//Lets you reroll 
	FALLING_STAR//Downward dash dealing damage based on distance traveled 
};

typedef struct {
	std::string title;
	std::string description;
	SDL_Rect src;
	Tag tag;
}Item;
#endif // !ITEM_H
