#include "game.hpp"

Game::Game() {
	//INIT SDL
	if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0){
		ERROR("Error init", SDL_GetError());
		exit(-1);
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
		exit(-1);
	}
	//END OF IMIT IMG

	window = SDL_CreateWindow("break", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
						   SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN);
	if(!window){
		ERROR("Error creating window", SDL_GetError());
		exit(-1);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(!renderer){
		ERROR("Error creating renderer", SDL_GetError());
		exit(-1);
	}
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);

	SDL_Texture *player_texture = IMG_LoadTexture(renderer, "assets/player_sheet.png");
	if(!player_texture){
		ERROR("Error loading player texture", IMG_GetError());
		exit(-1);
	}
	this->player.setTexture(player_texture);
	this->player.setXY(200, 300);

	this->screen = MAIN_MENU;

	this->paused = RESUME;
	this->menu = START;

	keys = SDL_GetKeyboardState(NULL);
}

void Game::draw_menu() {
}

void Game::menu_input() {
	if(screen == PAUSED){
		if(keys[SDL_SCANCODE_DOWN])
			paused = MAIN;
		else if(keys[SDL_SCANCODE_UP])
			paused = RESUME;

	}else{
		if(keys[SDL_SCANCODE_DOWN])
			switch(menu){
				case START:
					menu = OPTIONS;
				break;
				case OPTIONS:
					menu = QUIT;
				break;
				case QUIT:
					menu = START;
				break;
			}

		else if(keys[SDL_SCANCODE_UP])
			switch(menu){
				case START:
					menu = QUIT;
				break;
				case OPTIONS:
					menu = START;
				break;
				case QUIT:
					menu = OPTIONS;
				break;
			}
	}
}

void Game::loop() {
	if(screen == RUNNNING)
		player.update();
}

void Game::process() {
	switch (screen) {
		case RUNNNING:{
			if(keys[SDL_SCANCODE_ESCAPE]) screen = PAUSED;
			else player.input();
		}break;
		default:
			menu_input();
		break;
	}
}

void Game::render() {
	SDL_RenderClear(renderer);
	switch(screen){
		case RUNNNING:{
			player.draw(renderer);
		}break;
		default:{
			draw_menu();
		}break;
	}
	SDL_RenderPresent(renderer);
}

Game::~Game() {
	Mix_Quit();
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
