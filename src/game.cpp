#include "game.hpp"
#include "item.hpp"
#include "player.hpp"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
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

	text_texture = IMG_LoadTexture(renderer, "assets/text.png");
	if(!text_texture) {
		ERROR("Error loading the text texture", IMG_GetError());
		run = false;
	}

	items_texture = IMG_LoadTexture(renderer, "assets/item_sheet.png");
	if(!items_texture) {
		ERROR("Error loading the item sheet texture", IMG_GetError());
		run = false;
	}

	altar_texture = IMG_LoadTexture(renderer, "assets/altar.png");
	if(!altar_texture) {
		ERROR("Error loading the altar texture", IMG_GetError());
		run = false;
	}

	c_texture = IMG_LoadTexture(renderer, "assets/C.png");
	if(!altar_texture) {
		ERROR("Error loading the C texture", IMG_GetError());
		run = false;
	}

	this->altar_dst = {screen_width/2 - 32, FLOOR + 32, 64, 96};

	hp_texture = IMG_LoadTexture(renderer, "assets/hp.png");
	player.setHpTexture(hp_texture);

	max_item[0].title = "RED DICE";
	max_item[0].description = "Gives crit chance";
	max_item[0].src = {0, 0, 32, 32};
	max_item[0].tag = LUCKY_DIE;

	max_item[1].title = "DREAM CATCHER";
	max_item[1].description = "Deals return damage to enemies";
	max_item[1].src = {32, 0, 32, 32};
	max_item[1].tag = DREAM_CATCHER;

	max_item[2].title = "RABBIT FOOT";
	max_item[2].description = "Gives movement speed";
	max_item[2].src = {64, 0, 32, 32};
	max_item[2].tag = RABBITS_FOOT;

	max_item[3].title = "LADY BUG";
	max_item[3].description = "Gives double jump and \ndamage to enemies below";
	max_item[3].src = {96, 0, 32, 32};
	max_item[3].tag = LADY_BUG;
	
	max_item[4].title = "ACORN HELMET";
	max_item[4].description = "Gives +1 max hp";
	max_item[4].src = {128, 0, 32, 32};
	max_item[4].tag = ACORN_HELMET;

	max_item[5].title = "GOLD COIN";
	max_item[5].description = "Gives more coins";
	max_item[5].src = {160, 0, 32, 32};
	max_item[5].tag = LUCKY_COIN;

	max_item[6].title = "HORSESHOE";
	max_item[6].description = "Gives crit damage";
	max_item[6].src = {192, 0, 32, 32};
	max_item[6].tag = HORSESHOE;

	max_item[7].title = "EVIL EYE OF THE MARKET";
	max_item[7].description = "Discounts the shop";
	max_item[7].src = {224, 0, 32, 32};
	max_item[7].tag = EVIL_EYE;

	max_item[8].title = "GOLDEN CAT";
	max_item[8].description = "Deal more damge based on coins";
	max_item[8].src = {256, 0, 32, 32};
	max_item[8].tag = LUCKY_CAT;

	max_item[9].title = "WISHBONE";
	max_item[9].description = "Deal more base damage";
	max_item[9].src = {288, 0, 32, 32};
	max_item[9].tag = WISHBONE;

	max_item[10].title = "GOLD FISH";
	max_item[10].description = "Generate passive income";
	max_item[10].src = {320, 0, 32, 32};
	max_item[10].tag = GOLDFISH;

	max_item[11].title = "WINNING TICKET";
	max_item[11].description = "Get 5 in order to win";
	max_item[11].src = {352, 0, 32, 32};
	max_item[11].tag = WINNING_TICKET;

	max_item[12].title = "FALLING STAR";
	max_item[12].description = "Press S to slam into the ground";
	max_item[12].src = {384, 0, 32, 32};
	max_item[12].tag = FALLING_STAR;

	shop_option = 0;
	generate_shop();

	this->iter = 0;
	this->star_cooldown = 0;
	this->player_hit_cooldown = 0;
	this->max_enemies = 7;
	this->number_spawned = 0;
	this->num_tickets = 0;
	this->seconds = 0;
	this->enemy_spawn_hp = 30;
}

std::random_device rd;
std::default_random_engine gen(rd());
std::uniform_int_distribution<> d(80, 1720);

std::default_random_engine sh_gen(rd());
std::uniform_int_distribution<> sd(1, 13);

void Game::generate_shop() {
	for(int i=0;i<4;++i)
		item_shop[i] = &max_item[sd(sh_gen) % 13];
}

void Game::render_text(std::string s, int x, int y) {
	SDL_Rect dst = {x, y, 16, 16};
	SDL_Rect src = {0, 0, 16, 16};
	int m = 0;
	for(ul i=0;i<s.size();++i){
		dst.x = 16*m + x;
		if(s[i] == ' '){
			m++;
			continue;
		}
		if(s[i] == '\n'){
			m = 0;
			dst.y += 20;
			continue;
		}
		if(s[i] <= '9'){
			src.y = 32;
			src.x = 16*(s[i] - '0');
		}else if(s[i] <= 'Z'){
			src.x = 16*(s[i] - 'A');
			src.y = 16;
		}else{
			src.y = 0;
			src.x = 16*(s[i] - 'a');
		}
		++m;
		SDL_RenderCopy(renderer, text_texture, &src, &dst);
	}
}

void Game::draw_shop() {
	SDL_Rect dst = {screen_width/2 - 415, screen_height/2 - 270, 730, 640};
	SDL_RenderSetViewport(renderer, &dst);
	SDL_RenderCopy(renderer, shop_texture, NULL, NULL);

	SDL_Rect src = {0, 0, 540, 85};
	dst = {54, 115 + 54, 620, 105};
	for(int i=0; i<4; ++i){
		dst.y = i*115 + 54;
		SDL_RenderCopy(renderer, shop_row_texture, &src, &dst);
	}

	src = {0, 85, 540, 85};
	dst = {54, shop_option*115 + 54, 620, 105};
	SDL_RenderCopy(renderer, shop_row_texture, &src, &dst);

	dst.x = 74;
	dst.w = 64;
	dst.h = 64;
	for(int i=0; i<4; ++i){
		dst.y = i*120 + 54;
		SDL_RenderCopy(renderer, items_texture, &item_shop[i]->src, &dst);
		render_text(item_shop[i]->title, dst.x, dst.y + 70);
		render_text(item_shop[i]->description, dst.x + 70, dst.y + 15);
	}
	render_text("Coins:" + std::to_string(player.coins), 400, 600);
	SDL_RenderSetViewport(renderer, NULL);
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
				restart();
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

void Game::spawn_entity() {
	Walker *w = new Walker(d(gen), FLOOR, 32, 64, enemy_spawn_hp, walker_texture);
	entities.push_back(w);
	++number_spawned;
}

void Game::restart() {
	for(ul i = 0; i < entities.size(); ++i)
		delete entities[i];
	entities.resize(0);
	this->player.setXY(3*screen_width/4, 840);
	this->player.setHp(5);
	this->player.restart();
	this->screen = MAIN_MENU;
	this->option = START;
	this->number_spawned = 0;
	change_chosen(start_texture);
	this->num_tickets = 0;
}

void Game::loop() {
	if(screen == RUNNING){
		player.update();
		if(star_cooldown) --star_cooldown;
		if(player_hit_cooldown) --player_hit_cooldown;
		else SDL_SetTextureColorMod(player_texture, 255, 255, 255);
		if(!frame && number_spawned < max_enemies) spawn_entity();
		if(!(frame%30)){
			for(ul i = 0; i < entities.size(); ++i){
				entities[i]->process(player.x, player.y);
			}
		}
		if(!(frame%5)){
			for(ul i = 0; i < entities.size(); ++i){
				entities[i]->update();
				if(player_hit_cooldown == 0 && 
					check_colision(&player.dst, entities[i]->getDst())){

					--player.hp;
					if(!player.hp) restart();
					else player_hit_cooldown = 60;
					SDL_SetTextureColorMod(player_texture, 200, 0 ,128);
					Uint32 *items = player.getItems();
					if(items[DREAM_CATCHER]){
						for(ul i = 0; i < entities.size(); ++i)
							if(entities[i]->take_damage(items[DREAM_CATCHER]*10)){
								number_spawned--;
								entities.erase(entities.begin() + i);
								player.recieve_coins();
							}
					}
					return;
				}
			}
		}
		if(!(frame%12))
			player.generate_income();
	}
}

void Game::process() {
	switch (screen) {
		case RUNNING:{
			if(keys[SDL_SCANCODE_C]){
				if(check_colision(&player.dst, &altar_dst))
					screen = RUNNING_SHOP;
			}
			if(keys[SDL_SCANCODE_ESCAPE]){
				screen = PAUSED;
				option = START;
				change_chosen(resume_texture);
			}else if(keys[SDL_SCANCODE_S] && star_cooldown == 0){
				SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
				SDL_RenderClear(renderer);
				SDL_RenderPresent(renderer);
				SDL_Delay(200);
				star_cooldown = 60;
				Uint32 dmg = player.get_star_dmg();
				for(ul i = 0; i < entities.size(); ++i)
					if(entities[i]->take_damage(dmg)){
						number_spawned--;
						entities.erase(entities.begin() + i);
						player.recieve_coins();
					}
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
										number_spawned--;
										entities.erase(entities.begin() + i);
										player.recieve_coins();
									}
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
										number_spawned--;
										entities.erase(entities.begin() + i);
										player.recieve_coins();
									}
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
										number_spawned--;
										entities.erase(entities.begin() + i);
										player.recieve_coins();
									}
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
										number_spawned--;
										entities.erase(entities.begin() + i);
										player.recieve_coins();
									}
									player.vspeed = -20;
								}
							}
						}break;
						default:
							break;
					}
				}
			}
		}break;
		case RUNNING_SHOP:{
			if(keys[SDL_SCANCODE_ESCAPE]){
				screen = RUNNING;
			}
			if(keys[SDL_SCANCODE_UP])
				shop_option += (shop_option > 0)? -1:0;
			if(keys[SDL_SCANCODE_DOWN])
				shop_option += (shop_option < 3)? 1:0;
			if(keys[SDL_SCANCODE_RETURN]){
				screen = RUNNING;
				Uint32 *items = player.getItems();
				if(player.coins < 50 - items[EVIL_EYE]) return;
				player.coins -= 50-items[EVIL_EYE]*10;
				if(item_shop[shop_option]->tag == WINNING_TICKET){
					num_tickets++;
					if(num_tickets >= 5) win();
					generate_shop();
					return;
				}
				items[item_shop[shop_option]->tag]++;
				if(items[LUCKY_DIE] > 6) items[LUCKY_DIE] = 6;
				player.heal();
				generate_shop();
			}
		}break;
		default:
			menu_input();
		break;
	}
}

void Game::win() {
	screen = MAIN_MENU;
	restart();
	change_chosen(start_texture);
}

void Game::draw_altar() {
	SDL_Rect src = {0,0,32,48};
	SDL_RenderCopy(renderer, altar_texture, &src, &altar_dst);
}

void Game::render() {
	//draw the background
	SDL_RenderCopy(renderer, background_texture, NULL, NULL);

	switch(screen){
		case RUNNING:{
			for(ul i = 0; i < entities.size(); ++i){
				entities[i]->draw(renderer);
			}
			player.draw(renderer);
			player.draw_hud(renderer);
			draw_altar();
			if(check_colision(&player.dst, &altar_dst)){
				SDL_Rect dst = {altar_dst.x, altar_dst.y-96, 64, 64};
				SDL_RenderCopy(renderer, c_texture, NULL, &dst);
			}
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
	if(frame >= 61){
		frame = 0;
		++seconds;
		if(++iter == 10){
			iter = 0;
			enemy_spawn_hp *= 1.1;
		}
	}
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
	SDL_DestroyTexture(shop_texture);
	SDL_DestroyTexture(shop_row_texture);
	SDL_DestroyTexture(text_texture);
	SDL_DestroyTexture(items_texture);
	SDL_DestroyTexture(altar_texture);
	SDL_DestroyTexture(hp_texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
