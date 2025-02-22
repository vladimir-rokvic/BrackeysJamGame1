#include "game.hpp"
#include <SDL2/SDL_image.h>
#include <random>

Game::Game() {
	this->run = true;
	//INIT SDL
	if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0){
		ERROR("Error init", SDL_GetError());
		run = false;
	}
	//END OF INIT SDL

	//INIT SOUND SYSTEM
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
		ERROR("Failed to init Mix", Mix_GetError());
	}
	//END OF INIT SOUND SYSTEM

	//INIT IMG
	if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
		ERROR("Failed to init IMG", IMG_GetError());
		run = false;
	}
	//END OF IMIT IMG

	window = SDL_CreateWindow("break", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
						   SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN);
	if(!window){
		ERROR("Error creating window", SDL_GetError());
		run = false;
	}
	//Returns the size of the window in pixels
	SDL_GetWindowSizeInPixels(window, &screen_width, &screen_height);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(!renderer){
		ERROR("Error creating renderer", SDL_GetError());
		run = false;
	}
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);

	player_texture = IMG_LoadTexture(renderer, "assets/player_sheet.png");
	if(!player_texture){
		ERROR("Error loading player texture", IMG_GetError());
		run = false;
	}
	this->player.setTexture(player_texture);
	this->player.setXY(3*screen_width/4, 840);

	this->screen = MAIN_MENU;

	this->option = START;

	keys = SDL_GetKeyboardState(NULL);


	//LOADING MENU OPTIONS
	start_texture = IMG_LoadTexture(renderer, "assets/start_texture.png");
	if(!start_texture){
		ERROR("Error loading start text texture", IMG_GetError());
		run = false;
	}
	options_texture = IMG_LoadTexture(renderer, "assets/options_texture.png");
	if(!options_texture){
		ERROR("Error loading options text texture", IMG_GetError());
		run = false;
	}
	quit_texture = IMG_LoadTexture(renderer, "assets/quit_texture.png");
	if(!quit_texture){
		ERROR("Error loading quit text texture", IMG_GetError());
		run = false;
	}
	quit_texture = IMG_LoadTexture(renderer, "assets/quit_texture.png");
	if(!quit_texture){
		ERROR("Error loading quit text texture", IMG_GetError());
		run = false;
	}
	resume_texture = IMG_LoadTexture(renderer, "assets/resume_texture.png");
	if(!resume_texture){
		ERROR("Error loading resume text texture", IMG_GetError());
		run = false;
	}
	main_menu_texture = IMG_LoadTexture(renderer, "assets/main_menu_texture.png");
	if(!main_menu_texture){
		ERROR("Error loading main menu text texture", IMG_GetError());
		run = false;
	}

	chosen = start_texture;
	SDL_SetTextureColorMod(chosen, 200, 128, 00);
	//STOP LOADING MENU OPTIONS

	background_texture = IMG_LoadTexture(renderer, "assets/background_texture.png");
	if(!background_texture){
		ERROR("Error loading background texture", IMG_GetError());
		run = false;
	}

	//LOADING ENEMY TEXTURE
	walker_texture = IMG_LoadTexture(renderer, "assets/walker.png");
	if(!walker_texture){
		ERROR("Error loading walker texture", IMG_GetError());
		run = false;
	}

	shop_texture = IMG_LoadTexture(renderer, "assets/shop.png");
	if(!shop_texture){
		ERROR("Error loading the shop texture", IMG_GetError());
		run = false;
	}

	shop_row_texture = IMG_LoadTexture(renderer, "assets/item_stall.png");
	if(!shop_texture){
		ERROR("Error loading the shop row(stall) texture", IMG_GetError());
		run = false;
	}

	this->player_hit_cooldown = 0;
	this->max_enemies = 13;
	this->number_spawned = 0;
}

void Game::draw_shop() {
	SDL_Rect dst = {screen_width/2 - 315, screen_height/2 - 270, 630, 540};
	SDL_RenderSetViewport(renderer, &dst);
	SDL_RenderCopy(renderer, shop_texture, NULL, NULL);

	const SDL_Rect src = {0, 0, 540, 85};
	for(int i=0; i<4; ++i){
		dst = {20 + 34, i*(85+10) + 54, 520, 85};
		SDL_RenderCopy(renderer, shop_row_texture, &src, &dst);
	}
}

void Game::draw_menu() {
	if(screen == MAIN_MENU){
		SDL_Rect dst = {screen_width/2 - 176, screen_height/2 - 64, 160, 32};
		SDL_RenderCopy(renderer, start_texture, NULL, &dst);
		dst = {screen_width/2 - 192, screen_height/2 - 12, 224, 32};
		SDL_RenderCopy(renderer, options_texture, NULL, &dst);
		dst = {screen_width/2 - 144, screen_height/2 + 40, 128, 32};
		SDL_RenderCopy(renderer, quit_texture, NULL, &dst);
	}else if(screen == PAUSED){
		SDL_Rect dst = {screen_width/2 - 144, screen_height/2 - 64, 160, 32};
		SDL_RenderCopy(renderer, resume_texture, NULL, &dst);
		dst = {screen_width/2 - 188, screen_height/2 - 12, 224, 32};
		SDL_RenderCopy(renderer, main_menu_texture, NULL, &dst);
	}
}

void Game::change_chosen(SDL_Texture *texture){
	SDL_SetTextureColorMod(chosen, 255, 255, 255);
	chosen = texture;
	SDL_SetTextureColorMod(chosen, 200, 128, 00);
}

void Game::menu_input() {
	if(screen == PAUSED){
		if(keys[SDL_SCANCODE_DOWN]){
			option = QUIT;
			change_chosen(main_menu_texture);
		}
		else if(keys[SDL_SCANCODE_UP]){
			option = START;
			change_chosen(resume_texture);
		}
		else if(keys[SDL_SCANCODE_RETURN]){
			if(option == START){
				screen = RUNNING;
			}else{
				screen = MAIN_MENU;
				option = START;
				change_chosen(start_texture);
			}
		}

	}else{
		if(keys[SDL_SCANCODE_DOWN]){
			switch(option){
				case START:
					option = OPTIONS;
					change_chosen(options_texture);
				break;
				case OPTIONS:
					option = QUIT;
					change_chosen(quit_texture);
				break;
				case QUIT:
					option = START;
					change_chosen(start_texture);
				break;
			}
		}

		else if(keys[SDL_SCANCODE_UP]){
			switch(option){
				case START:
					option = QUIT;
					change_chosen(quit_texture);
				break;
				case OPTIONS:
					option = START;
					change_chosen(start_texture);
				break;
				case QUIT:
					option = OPTIONS;
					change_chosen(options_texture);
				break;
			}
		}

		else if(keys[SDL_SCANCODE_RETURN]){
			switch(option){
				case START:{
					//TODO: Change back later to a normal running loop
					//screen = RUNNING
					screen = RUNNING_SHOP;
				}break;
				case OPTIONS:{
					//option = START;
					//screen = SCREEN_OPTIONS;
				}break;
				case QUIT:{
					run = false;
				}break;
			}
		}
	}
}

//Compares 2 destination rects and returns wheter theyre colliding
bool Game::check_colision(const SDL_Rect *dst1, const SDL_Rect *dst2) {
	const bool x_collision = ((dst1->x >= dst2->x) && (dst1->x <= (dst2->x + dst2->w)))
		|| ((dst2->x >= dst1->x) && (dst2->x <= (dst1->x + dst1->w)));
	const bool y_collision = ((dst1->y >= dst2->y) && (dst1->y <= (dst2->y + dst2->h)))
		|| ((dst2->y >= dst1->y) && (dst2->y <= (dst1->y + dst1->h)));
	if(x_collision && y_collision) return true;
	return false;
}

std::random_device rd;
std::default_random_engine gen(rd());
std::uniform_int_distribution<> d(80, 1720);

void Game::spawn_entity() {
	Walker *w = new Walker(d(gen), FLOOR, 32, 64, walker_texture);
	entities.push_back(w);
	++number_spawned;
}

void Game::restart() {
	for(ul i = 0; i < entities.size(); ++i)
		delete entities[i];
	entities.resize(0);
	this->player.setXY(3*screen_width/4, 840);
	this->player.setHp(5);
	this->screen = MAIN_MENU;
	this->option = START;
}

void Game::loop() {
	if(screen == RUNNING){
		player.update();
		if(player_hit_cooldown) --player_hit_cooldown;
		else SDL_SetTextureColorMod(player_texture, 255, 255, 255);
		if(!frame && number_spawned < max_enemies) spawn_entity();
		if(!(frame%30)){
			for(ul i = 0; i < entities.size(); ++i){
				entities[i]->process(player.x, player.y);
			}
		}
		if(!(frame%4)){
			for(ul i = 0; i < entities.size(); ++i){
				entities[i]->update();
				if(player_hit_cooldown == 0 && 
					check_colision(&player.dst, entities[i]->getDst())){

					--player.hp;
					if(!player.hp) restart();
					else player_hit_cooldown = 30;
					SDL_SetTextureColorMod(player_texture, 200, 0 ,128);
					return;
				}
			}
		}
	}
}

void Game::process() {
	switch (screen) {
		case RUNNING:{
			if(keys[SDL_SCANCODE_ESCAPE]){
				screen = PAUSED;
				option = START;
				change_chosen(resume_texture);
			}else{
				//PROCCESING ATTACK
				if(player.input() == ATTACKING){
					switch(player.direction){
						case UP:{
							const SDL_Rect dst = {
								player.x - 32,
								player.y - 48, 
								128, 
								128
							};
							for(ul i = 0; i < entities.size(); ++i){
								if(check_colision(entities[i]->getDst(), &dst)){
									if(entities[i]->take_damage(player.get_damage())){
										entities.erase(entities.begin() + i);
										player.coins+=10;
									}
									break;
								}
							}
						}break;
						case RIGHT:{
							const SDL_Rect dst = {
								player.x + 48,
								player.y, 
								128, 
								128
							};
							for(ul i = 0; i < entities.size(); ++i){
								if(check_colision(&dst, entities[i]->getDst())){
									if(entities[i]->take_damage(player.get_damage())){
										entities.erase(entities.begin() + i);
										player.coins+=10;
									}
									break;
								}
							}
						}break;
						case LEFT:{
							const SDL_Rect dst = {
								player.x - 112,
								player.y, 
								128, 
								128
							};
							for(ul i = 0; i < entities.size(); ++i){
								if(check_colision(entities[i]->getDst(), &dst)){
									if(entities[i]->take_damage(player.get_damage())){
										entities.erase(entities.begin() + i);
										player.coins+=10;
									}
									break;
								}
							}
						}break;
						case DOWN:{
							const SDL_Rect dst = {
								player.x - 32,
								player.y + 48, 
								128, 
								128
							};
							for(ul i = 0; i < entities.size(); ++i){
								if(check_colision(entities[i]->getDst(), &dst)){
									if(entities[i]->take_damage(player.get_damage())){
										entities.erase(entities.begin() + i);
										player.coins+=10;
									}
									break;
								}
							}
						}break;
						default:
							break;
					}
				}
			}
		}break;
		default:
			menu_input();
		break;
	}
}

void Game::render() {
	//draw the background
	SDL_RenderCopy(renderer, background_texture, NULL, NULL);

	switch(screen){
		case RUNNING:{
			for(ul i = 0; i < entities.size(); ++i)
				entities[i]->draw(renderer);
			player.draw(renderer);
		}break;
		case RUNNING_SHOP:{
			draw_shop();
		}break;
		default:{
			draw_menu();
		}break;
	}

	SDL_RenderPresent(renderer);
	++frame;
	frame = (frame >= 61)? 0:frame;
}

Game::~Game() {
	for(ul i = 0;i < entities.size();++i)
		delete entities[i];

	Mix_Quit();
	IMG_Quit();
	SDL_DestroyTexture(start_texture);
	SDL_DestroyTexture(options_texture);
	SDL_DestroyTexture(quit_texture);
	SDL_DestroyTexture(resume_texture);
	SDL_DestroyTexture(main_menu_texture);
	SDL_DestroyTexture(background_texture);
	SDL_DestroyTexture(walker_texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
