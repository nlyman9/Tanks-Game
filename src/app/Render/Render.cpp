#include <vector>
#include <string>
#include <time.h>

#include "Render.hpp"
Render::~Render() {
  close();
}

bool Render::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
	}

	gWindow = SDL_CreateWindow("Tanks", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == nullptr)
	{
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}
	#if __APPLE__
	#else
	gScreenSurface = SDL_GetWindowSurface( gWindow );
	#endif

	// Set up rendered with out vsync
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == nullptr)
	{
		std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	if( TTF_Init() == -1 )
	{
		printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
		return false;
	}
	
	fontRoboto = TTF_OpenFont("src/res/fonts/Roboto.ttf", 24);
	if (fontRoboto == NULL) {
        fprintf(stderr, "error: Roboto not found\n");
        exit(EXIT_FAILURE);
    }

	// Set initalization for text
	fontSansUntertale = TTF_OpenFont("src/res/fonts/SansUndertale.ttf", 24);
	if (fontSansUntertale == NULL) {
        fprintf(stderr, "error: SansUndertale not found\n");
        exit(EXIT_FAILURE);
    }

	// Set renderer draw/clear color
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	gTileSheet = loadImage("src/res/images/tiles.png", gRenderer);
	for (int i = 0; i < 3; i++) {
			gTileRects[i].x = i * TILE_SIZE;
			gTileRects[i].y = 0;
			gTileRects[i].w = TILE_SIZE;
			gTileRects[i].h = TILE_SIZE;
	}

	// Set image fir cursor
	crosshair = loadImage("src/res/images/cursor.png", gRenderer);

	white = {255, 255, 255, 0};

	return true;
}

void Render::close() {
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	// Quit SDL subsystems
	SDL_Quit();
}

int Render::drawMenu() {

	SDL_Texture* menuNone = loadImage("src/res/images/menu_none.png", gRenderer);
	SDL_Texture* menuSinglePlayer = loadImage("src/res/images/menu_single_player.png", gRenderer);
	SDL_Texture* menuMultiPlayer = loadImage("src/res/images/menu_multi_player.png", gRenderer);
	SDL_Texture* menuCredits = loadImage("src/res/images/menu_credits.png", gRenderer);

	bool quit = false;
	int menuOption = MENU_NONE;

	SDL_Rect singlePlayerBox = {452, 144, 377, 111};
	SDL_Rect multiPlayerBox = {452, 288, 377, 111};
	SDL_Rect creditsBox = {452, 435, 377, 111};
  	while(!quit) {
		SDL_Event e;

		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT || (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE)) {
				quit = true;
				return -1;
			} else if(e.type == SDL_KEYDOWN) {
				if(menuOption == MENU_NONE) {
					menuOption = MENU_SINGLE;
				}
				switch(e.key.keysym.sym) {
					case SDLK_DOWN:
						menuOption++;
						break;
					case SDLK_s:
						menuOption++;
						break;
					case SDLK_UP:
						menuOption--;
						break;
					case SDLK_w:
						menuOption--;
						break;
					case SDLK_RETURN:
						return menuOption;
						break;
				} 
			} else if(e.type == SDL_MOUSEBUTTONDOWN) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				SDL_Rect clickBox = {x, y, 1, 1};
				SDL_Rect intersection;

				// Check for box clicks
				if(SDL_IntersectRect(&clickBox, &singlePlayerBox, &intersection)) {
					return MENU_SINGLE;
				}
				if(SDL_IntersectRect(&clickBox, &multiPlayerBox, &intersection)) {
					return MENU_MULTI;
				}
				if(SDL_IntersectRect(&clickBox, &creditsBox, &intersection)) {
					return MENU_CREDITS;
				}
			}
		}

		if(menuOption == -1) {
			menuOption = MENU_CREDITS;
		}

		if(menuOption == 3) {
			menuOption = MENU_SINGLE;
		}

		SDL_Rect fullscreen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
		if(menuOption == MENU_SINGLE) {
			SDL_RenderCopy(gRenderer, menuSinglePlayer, NULL, &fullscreen); 
		} else if(menuOption == MENU_MULTI) {
			SDL_RenderCopy(gRenderer, menuMultiPlayer, NULL, &fullscreen); 
		} else if(menuOption == MENU_CREDITS) {
			SDL_RenderCopy(gRenderer, menuCredits, NULL, &fullscreen); 
		} else {
			SDL_RenderCopy(gRenderer, menuNone, NULL, &fullscreen); 
		}

		int cursorX = 0, cursorY = 0;

		if(e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) {
			SDL_GetMouseState(&cursorX, &cursorY);
		}

		SDL_Rect cursorRect = {cursorX - CROSSHAIR_SIZE/2, cursorY - CROSSHAIR_SIZE/2, CROSSHAIR_SIZE, CROSSHAIR_SIZE};
		SDL_RenderCopy(gRenderer, crosshair, NULL, &cursorRect);

		SDL_RenderPresent(gRenderer);
	}

	return menuOption;
}
int Render::present(){
	SDL_RenderPresent( gRenderer );
}
int Render::drawBackground(){
	SDL_RenderClear( gRenderer );
	SDL_Rect fullscreen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	SDL_Texture* background = loadImage("src/res/images/MultiplayerScreen.png", gRenderer);
	SDL_RenderCopy(gRenderer, background, NULL, &fullscreen);
}
int Render::drawText(Box* box, const std::string* toDraw, int XOFFSET, int YOFFSET, int WIDTH, int HEIGHT){
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(fontRoboto, toDraw->c_str(), textColor);
	Message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);
	SDL_Rect* rect = new SDL_Rect();
	rect->x = box->getRectangle()->x + XOFFSET;
	rect->y = box->getRectangle()->y + YOFFSET;
	rect->w = WIDTH;
	rect->h = HEIGHT;
	SDL_RenderCopy(gRenderer, Message, NULL, rect);
	//clean up memory
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);
	delete rect;
}
int Render::drawBox(Box toDraw){
	switch(toDraw.getType()){
		case BUTTON:
			return drawButton(toDraw);
		case TEXT:
			return drawTextField(toDraw);
	}
	return 0;
}
int Render::drawButton(Box toDraw){
	if(toDraw.getIMGPath() == ""){ //if src is empty
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF); //default color
		SDL_RenderFillRect(gRenderer, toDraw.getRectangle());
	}else{
		SDL_Texture* button = loadImage(toDraw.getIMGPath(), gRenderer);
		SDL_RenderCopy(gRenderer, button, NULL, toDraw.getRectangle());
	}
	return 0;
}
int Render::drawTextField(Box toDraw){
	if(toDraw.getIMGPath() == ""){ //if src is empty
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF); //default color
		SDL_RenderFillRect(gRenderer, toDraw.getRectangle());
	}else{
		SDL_Texture* button = loadImage(toDraw.getIMGPath(), gRenderer);
		SDL_RenderCopy(gRenderer, button, NULL, toDraw.getRectangle());
	}
	SDL_Rect* TextField = new SDL_Rect();
	TextField->x = toDraw.getRectangle()->x + toDraw.getRectangle()->w + toDraw.TEXT_FIELD_OFFSET();
	TextField->y = toDraw.getRectangle()->y;
	TextField->w = toDraw.TEXT_FIELD_WIDTH();
	TextField->h = toDraw.getRectangle()->h;
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x80, 0xFF); //default textfield color
	SDL_RenderFillRect(gRenderer, TextField);
	delete TextField;
	return 0;
}
void Render::setTileMap(std::vector<std::vector<int>>* tileMap) {
	tile_map = *tileMap;
}

int Render::draw(double update_lag) {
	int c;
	SDL_Rect cur_out;

	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(gRenderer);

	// Render array of tiles
	for (int x = BORDER_GAP + TILE_SIZE, i = 0; x < SCREEN_WIDTH - BORDER_GAP - TILE_SIZE; x+=TILE_SIZE, i++) {
		for (int y = TILE_SIZE, j = 0; y < SCREEN_HEIGHT - TILE_SIZE; y+=TILE_SIZE, j++) {
			cur_out = { x, y, TILE_SIZE, TILE_SIZE};
			SDL_RenderCopy(gRenderer, gTileSheet, &gTileRects[tile_map[i][j]], &cur_out);
		}
	}

	//GENERATES TOP BORDER
	c = BORDER_GAP;
	while (c < SCREEN_WIDTH - BORDER_GAP) {
		cur_out = {c, 0, TILE_SIZE, TILE_SIZE};
		SDL_RenderCopy(gRenderer, gTileSheet, &gTileRects[2], &cur_out);
		c += TILE_SIZE;
	}
	//GENERATES LEFT BORDER
	c = 0;
	while (c < SCREEN_HEIGHT) {
		cur_out = {BORDER_GAP, c, TILE_SIZE, TILE_SIZE};
		SDL_RenderCopy(gRenderer, gTileSheet, &gTileRects[2], &cur_out);
		c += TILE_SIZE;
	}
	//GENERATES BOTTOM BORDER
	c = BORDER_GAP;
	while (c < SCREEN_WIDTH - BORDER_GAP) {
		cur_out = {c, SCREEN_HEIGHT - TILE_SIZE, TILE_SIZE, TILE_SIZE};
		SDL_RenderCopy(gRenderer, gTileSheet, &gTileRects[2], &cur_out);
		c += TILE_SIZE;
	}
	//GENERATES RIGHT BORDER
	c = 0;
	while (c < SCREEN_HEIGHT) {
		cur_out = {SCREEN_WIDTH - TILE_SIZE - BORDER_GAP, c, TILE_SIZE, TILE_SIZE};
		SDL_RenderCopy(gRenderer, gTileSheet, &gTileRects[2], &cur_out);
		c += TILE_SIZE;
	}

	//Render timer
	surfaceMessage = TTF_RenderText_Solid(fontRoboto, (std::to_string(timer)).c_str(), white);
	timer_display = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);
	SDL_FreeSurface( surfaceMessage ); //free the surface that gets created
	timer_box.x = BORDER_GAP + 13 * TILE_SIZE - TILE_SIZE/2;
	timer_box.y = 0;
	timer_box.w = 48;
	timer_box.h = 48;
	SDL_RenderCopy(gRenderer, timer_display, NULL, &timer_box);
	SDL_DestroyTexture( timer_display );


	// Render player
	for (auto& player : gPlayers) {
		player->draw(gRenderer, update_lag);
	}

	// Render all the enemies
	for (auto& enemy : gEnemies) {
		enemy->draw(gRenderer, update_lag);
	}

	// Render all projectiles
	for (auto& projectile : gProjectiles) {
		projectile->draw(gRenderer, update_lag);
	}

	// Render all bombs
	for (auto& bomb : gBombs) {
		bomb->draw(gRenderer, update_lag);
	}

	// Draw the cursor on the screen
	SDL_Rect cursorRect = {cursorX - CROSSHAIR_SIZE/2, cursorY - CROSSHAIR_SIZE/2, CROSSHAIR_SIZE, CROSSHAIR_SIZE};

	SDL_RenderCopy(gRenderer, crosshair, NULL, &cursorRect);

	SDL_RenderPresent(gRenderer);

	// Success?
	return 0;
}

SDL_Renderer* Render::getRenderer() {
	return gRenderer;
}

void Render::setPlayer(std::vector<Player *> players) {
	gPlayers = players;
}

void Render::setPlayerEnemies(std::vector<Player *> players) {
	gPlayers.insert(gPlayers.end(), players.begin(), players.end());
}

void Render::setEnemies(std::vector<Enemy *> enemies) {
	gEnemies = enemies;
}

void Render::setProjectiles(std::vector<Projectile *> projectiles) {
	gProjectiles = projectiles;
}

void Render::setBombs(std::vector<Bomb *> bombs) {
	gBombs = bombs;
}

void Render::setTimer(unsigned int passed_timer) {
	timer = passed_timer;
}

void Render::clear() {
	gPlayers.clear();
	gEnemies.clear();
	gProjectiles.clear();
	gBombs.clear();
}