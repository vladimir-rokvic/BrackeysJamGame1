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
	//Returns the size of the window in pixels
	SDL_GetWindowSizeInPixels(window, &screen_width, &screen_height);

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

	this->option = START;

	keys = SDL_GetKeyboardState(NULL);

	run = true;

	//LOADING MENU OPTIONS
	start_texture = IMG_LoadTexture(renderer, "assets/start_texture.png");
	if(!start_texture){
		ERROR("Error loading start text texture", IMG_GetError());
		exit(-1);
	}
	options_texture = IMG_LoadTexture(renderer, "assets/options_texture.png");
	if(!options_texture){
		ERROR("Error loading options text texture", IMG_GetError());
		exit(-1);
	}
	quit_texture = IMG_LoadTexture(renderer, "assets/quit_texture.png");
	if(!quit_texture){
		ERROR("Error loading quit text texture", IMG_GetError());
		exit(-1);
	}
	quit_texture = IMG_LoadTexture(renderer, "assets/quit_texture.png");
	if(!quit_texture){
		ERROR("Error loading quit text texture", IMG_GetError());
		exit(-1);
	}
	resume_texture = IMG_LoadTexture(renderer, "assets/resume_texture.png");
	if(!resume_texture){
		ERROR("Error loading quit text texture", IMG_GetError());
		exit(-1);
	}
	main_menu_texture = IMG_LoadTexture(renderer, "assets/main_menu_texture.png");
	if(!main_menu_texture){
		ERROR("Error loading quit text texture", IMG_GetError());
		exit(-1);
	}

	chosen = start_texture;
	SDL_SetTextureColorMod(chosen, 200, 128, 00);
	//STOP LOADING MENU OPTIONS
}

void Game::draw_menu() {
	if(screen == MAIN_MENU){
		SDL_Rect dst = {screen_width/2 - 160, screen_height/2 - 64, 160, 32};
		SDL_RenderCopy(renderer, start_texture, NULL, &dst);
		dst = {screen_width/2 - 192, screen_height/2 - 12, 224, 32};
		SDL_RenderCopy(renderer, options_texture, NULL, &dst);
		dst = {screen_width/2 - 144, screen_height/2 + 40, 128, 32};
		SDL_RenderCopy(renderer, quit_texture, NULL, &dst);
	}else if(screen == PAUSED){
		SDL_Rect dst = {screen_width/2 - 192, screen_height/2 - 64, 160, 32};
		SDL_RenderCopy(renderer, resume_texture, NULL, &dst);
		dst = {screen_width/2 - 220, screen_height/2 - 12, 224, 32};
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
				screen = RUNNNING;
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
					screen = RUNNNING;
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

void Game::loop() {
	if(screen == RUNNNING)
		player.update();
}

void Game::process() {
	switch (screen) {
		case RUNNNING:{
			if(keys[SDL_SCANCODE_ESCAPE]){
				screen = PAUSED;
				option = START;
				change_chosen(resume_texture);
			}
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
	SDL_DestroyTexture(start_texture);
	SDL_DestroyTexture(options_texture);
	SDL_DestroyTexture(quit_texture);
	SDL_DestroyTexture(background_texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
