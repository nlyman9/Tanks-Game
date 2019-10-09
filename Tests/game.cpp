#include <iostream>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <math.h>

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;
constexpr int TILE_SIZE = 48;
constexpr int BORDER_GAP = 16;
constexpr int OBST_WIDTH = 70;
constexpr int OBST_HEIGHT = 40;
constexpr int OBST_1_WIDTH = 150;
constexpr int OBST_1_HEIGHT = 150;
constexpr int OBST_2_WIDTH = 20;
constexpr int OBST_2_HEIGHT = 100;
constexpr int BOX_WIDTH = 20;
constexpr int BOX_HEIGHT = 20;
constexpr int MAX_VELOCITY = 1;

// Function declarations
bool init();
SDL_Texture* loadImage(std::string fname);
bool check_vicinity(SDL_Rect* a, SDL_Rect* b);
bool player_vicinity(SDL_Rect* a, SDL_Rect* b);
int** generateLineMap();
int** generateMirrorMap();
void close();

// Globals
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Texture* gTileSheet;
SDL_Texture* gTank_Red;
SDL_Texture* gTank_Blue;
SDL_Rect gTileRects[3];
//true for testing bottom right to top left, false top left to bottom right
bool dir = true;
int** tile_map;
std::vector<SDL_Texture*> gTex;
SDL_Rect cur_out;


bool init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
	}

	gWindow = SDL_CreateWindow("TANKS!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == nullptr) {
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return  false;
	}

	// Adding VSync to avoid absurd framerates
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (gRenderer == nullptr) {
		std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return  false;
	}

	int imgFlags = IMG_INIT_PNG;
	int retFlags = IMG_Init(imgFlags);
	if(retFlags != imgFlags) {
		std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
		return false;
	}

	// Set renderer draw/clear color
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);

	gTileSheet = loadImage("source/res/images/tiles.png");
	for (int i = 0; i < 3; i++) {
			gTileRects[i].x = i * TILE_SIZE;
			gTileRects[i].y = 0;
			gTileRects[i].w = TILE_SIZE;
			gTileRects[i].h = TILE_SIZE;
	}

	// Fill 2D tile array of tiles with all 0s
	// int** array = 0;
	tile_map = new int*[24];

	for(int j = 0; j < 24; j++)
	{
		tile_map[j] = new int[13];
		for(int h = 0; h < 13; h++)
		{
			tile_map[j][h] = 0;
		}
	}

	//small randomly generated thing

	// Select map generation technique
	enum map_types { destructible, holes, line, maze, mirror };
	srand(time(NULL));

	// switch(rand() % 4)
	switch(line)
	{
		case destructible:
			tile_map[4][4] = 2;
			break;
		case holes:
			tile_map[1][1] = 2;
			break;
		case line:
			tile_map = generateLineMap();
			break;
		case maze:
			tile_map[6][10] = 2;
			break;
		case mirror:
			tile_map = generateMirrorMap();
			tile_map[14][10] = 2;
			break;
	}



	return true;
}



SDL_Texture* loadImage(std::string fname) {
	SDL_Texture* newText = nullptr;

	SDL_Surface* startSurf = IMG_Load(fname.c_str());
	if (startSurf == nullptr) {
		std::cout << "Unable to load image " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	newText = SDL_CreateTextureFromSurface(gRenderer, startSurf);
	if (newText == nullptr) {
		std::cout << "Unable to create texture from " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
	}

	SDL_FreeSurface(startSurf);

	return newText;
}


void close() {
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	SDL_DestroyTexture(gTileSheet);
	SDL_DestroyTexture(gTank_Red);
	SDL_DestroyTexture(gTank_Blue);
	gTileSheet = nullptr;
	gTank_Red = nullptr;
	gTank_Blue = nullptr;

	// Quit SDL subsystems
	SDL_Quit();
}

int** generateLineMap() {
	// make empty room array
	int** array = 0;
    array = new int*[24];

    for(int i = 0; i < 24; i++)
    {
        array[i] = new int[13];
        for(int j = 0; j < 13; j++)
        {
            array[i][j] = 0;
        }
    }

	int** room = array;

	//initialize random seed
	srand(time(NULL));
	int random_index;
	int random_index_bonus;
	for(int i = 0; i < 13; i++) {
		random_index = rand() % 24;
		if(rand() % 3 == 0)
		{
			random_index_bonus = rand() % 24;
			if(abs(random_index_bonus - random_index) == 1)
			{
				random_index_bonus = -1;
			}
		}
		else
		{
			random_index_bonus = -1;
		}


		for(int j = 0; j < 24; j++) {
			if(i % 2 != 0) {
				if(j == random_index || j == random_index_bonus)
					room[j][i] = 0;
				else
					room[j][i] = 2;
			}
			else
				room[j][i] = 0;
		}
	}
	return room;
}

int** generateMirrorMap() //tile_map is 24x13
{
    int** array = 0;
    array = new int*[24];

    for(int i = 0; i < 24; i++)
    {
        array[i] = new int[13];
        for(int j = 0; j < 13; j++)
        {
            array[i][j] = 0;
        }
    }

    srand(time(NULL));

    array[13][10] = 2;

    return array;
}

bool checkDistFrom(int playX, int playY, int enemX, int enemY) {

	double  stepOne = (double) (pow((playX - enemX), 2)+pow((playY - enemY), 2));

 	double distanceAway =  (pow(stepOne, .5));

	if(distanceAway < 200.0) {
		return true;
	}
	return false;
}

//Evaluates to true if a Rect a edge is within 8 pixels of an edge of Rect b
bool check_vicinity(SDL_Rect* a, SDL_Rect* b) {
	// Check vertical overlap
	if (a->y + a->h <= b->y - 8)
		return false;
	if (a->y >= b->y + b->h + 8)
		return false;

	// Check horizontal overlap
	if (a->x >= b->x + b->w + 8)
		return false;
	if (a->x + a->w <= b->x - 8)
		return false;

	// Must overlap in both
	return true;
}

//Return true if rect a is colliding with rect b
bool player_vicinity(SDL_Rect* a, SDL_Rect* b) {
	// Check vertical overlap
	if (a->y + a->h <= b->y)
		return false;
	if (a->y >= b->y + b->h)
		return false;

	// Check horizontal overlap
	if (a->x >= b->x + b->w)
		return false;
	if (a->x + a->w <= b->x)
		return false;

	// Must overlap in both
	return true;
}

//check if coordinate x or coordinate y results to box being along a wall
bool checkWall(int x, int y) {

	//left wall
	if(x <= 20) {
		return true;
	}
	//right wall
	else if(x >= SCREEN_WIDTH - 2*BOX_WIDTH) {
		return true;
	}
	//top wall
	else if(y <= 20) {
		return true;
	}
	//bottom wall
	else if(y >= SCREEN_HEIGHT - 2*BOX_HEIGHT) {
		return true;
	}
	else {
		return false;

	}
}

int main() {
	if (!init()) {
		std::cout <<  "Failed to initialize!" << std::endl;
		close();
		return 1;
	}

	gTileSheet = loadImage("source/res/images/tiles.png");
	gTank_Red = loadImage("source/res/images/red_tank.png");
	gTank_Blue = loadImage("source/res/images/blue_tank.png");

	for (int i = 0; i < 3; i++) {
		gTileRects[i].x = i * TILE_SIZE;
		gTileRects[i].y = 0;
		gTileRects[i].w = TILE_SIZE;
		gTileRects[i].h = TILE_SIZE;
	}

	SDL_Event e;
	bool gameon = true;
	int c;
	SDL_Rect cur_out;

	// Current position to render the box
	// Start off with it in the middle
	int x_pos = 75;
	int y_pos = 60;

	//Start position of obstacle - middle
	int x_obst_pos = SCREEN_WIDTH/2 - OBST_WIDTH/2;
	int y_obst_pos = SCREEN_HEIGHT/2 - OBST_HEIGHT/2;
	//initialize OBST rect
	SDL_Rect obst = {x_obst_pos, y_obst_pos, OBST_WIDTH, OBST_HEIGHT};

	//start position of OBST_1
	int x_obst_1_pos = 900;
	int y_obst_1_pos = 450;
	//Initialize OBST_1 rect
	SDL_Rect obst_1 = {x_obst_1_pos, y_obst_1_pos, OBST_1_WIDTH, OBST_1_HEIGHT};

	//start position of OBST_2
	int x_obst_2_pos = 200;
	int y_obst_2_pos = 150;
	//Initialize OBST_2 rect
	SDL_Rect obst_2 = {x_obst_2_pos, y_obst_2_pos, OBST_2_WIDTH, OBST_2_HEIGHT};

	//Enemy box start position
	int x_enemy_pos = SCREEN_WIDTH - BOX_WIDTH/2 - 75;
	int y_enemy_pos = SCREEN_HEIGHT - BOX_HEIGHT/2 - 60;
	int enemy_start_x = x_enemy_pos;
	int enemy_start_y = y_enemy_pos;
	//Initialize enemy box
	SDL_Rect enemy_box = {x_enemy_pos, y_enemy_pos, BOX_WIDTH, BOX_HEIGHT};

	// Current velocity of the box
	// Start off at reset
	int x_vel = 0;
	int y_vel = 0;


	while(gameon) {
		while(SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				gameon = false;
			}

			else if(e.type == SDL_KEYDOWN) {
				switch(e.key.keysym.sym) {
					case SDLK_w:
						y_vel -= MAX_VELOCITY;
						break;

					case SDLK_a:
						x_vel -= MAX_VELOCITY;
						break;

					case SDLK_s:
						y_vel += MAX_VELOCITY;
						break;

					case SDLK_d:
						x_vel += MAX_VELOCITY;
						break;
				}
			}
			else if(e.type == SDL_KEYUP) {
				switch(e.key.keysym.sym) {
					case SDLK_w:
						y_vel = 0;
						break;
					case SDLK_a:
						x_vel = 0;
						break;
					case SDLK_s:
						y_vel = 0;
						break;
					case SDLK_d:
						x_vel = 0;
						break;
				}
			}
		}

		// Move box
		if(x_vel > MAX_VELOCITY) {
			x_vel = MAX_VELOCITY;
		}
		if(x_vel < -MAX_VELOCITY) {
			x_vel = -MAX_VELOCITY;
		}
		if(y_vel > MAX_VELOCITY) {
			y_vel = MAX_VELOCITY;
		}
		if(y_vel < -MAX_VELOCITY) {
			y_vel = -MAX_VELOCITY;
		}
		x_pos += x_vel;
		y_pos += y_vel;
		//Ensure box doesn't collide with obstacles
		SDL_Rect player_collide = {x_pos, y_pos, BOX_WIDTH, BOX_HEIGHT};
		if (player_vicinity(&player_collide, &obst) || player_vicinity(&player_collide, &obst_1) || player_vicinity(&player_collide, &obst_2)) {
			x_pos -= x_vel;
			y_pos -= y_vel;
		}

		//Prevent the box from going offscreen
		if(x_pos > SCREEN_WIDTH - BOX_WIDTH) {
			x_pos = SCREEN_WIDTH - BOX_WIDTH;
		}
		if(x_pos < 0){
			x_pos = 0;
		}
		if(y_pos < 0){
			y_pos = 0;
		}
		if(y_pos > SCREEN_HEIGHT - BOX_HEIGHT) {
			y_pos = SCREEN_HEIGHT - BOX_HEIGHT;
		}

		//Checking if enemy should move away
		//bool retreat;
		//retreat = checkDistFrom(x_pos, y_pos, x_enemy_pos, y_enemy_pos);


		bool nearWall;
		nearWall = checkWall(x_enemy_pos, y_enemy_pos);

		//Make enemy avoid player when player gets too close and return to center otherwise
		/*-----------------------------------------------------------------------------------
		if(!retreat) {

			//go to center

			if(x_enemy_pos <= SCREEN_WIDTH/2) {
				x_enemy_pos += MAX_VELOCITY;
			}
			else{
				x_enemy_pos += -MAX_VELOCITY;
			}
			if(y_enemy_pos <= SCREEN_HEIGHT/2) {
				y_enemy_pos += MAX_VELOCITY;
			}
			else{
				y_enemy_pos += -MAX_VELOCITY;
			}

		}
		else{

			//run away but not near wall
			if(!nearWall) {
				if(x_pos >= x_enemy_pos) {
					x_enemy_pos += -MAX_VELOCITY;
				}
				else{
					x_enemy_pos += MAX_VELOCITY;
				}
				if(y_pos >= y_enemy_pos) {
					y_enemy_pos += -MAX_VELOCITY;
				}
				else{
					y_enemy_pos += MAX_VELOCITY;
				}
			}
			else {
			//run away and on wall


			if(x_enemy_pos == 20 || x_enemy_pos == SCREEN_WIDTH - 2*BOX_WIDTH){
				if(y_pos >= y_enemy_pos) {
											y_enemy_pos += -MAX_VELOCITY;
									}
									else {
											y_enemy_pos += MAX_VELOCITY;
									}


			}
			if(y_enemy_pos == 20 || y_enemy_pos == SCREEN_HEIGHT - 2*BOX_HEIGHT) {

				if(x_pos >= x_enemy_pos) {
											x_enemy_pos += -MAX_VELOCITY;
									}
									else{
											x_enemy_pos += MAX_VELOCITY;
									}

			}
		}
		---------------------------------------------------------------------------------*/

	//Make enemy move from bottom right to upper left while avoiding obstacles
		//Move enemy box to the top left
		if (dir) {
			if (x_enemy_pos != 75)
				x_enemy_pos += -MAX_VELOCITY;
			if (y_enemy_pos != 60)
				y_enemy_pos += -MAX_VELOCITY;
		}
		//Move enemy box to the bottom right
		else {
			if (x_enemy_pos != enemy_start_x)
				x_enemy_pos += MAX_VELOCITY;
			if (y_enemy_pos != enemy_start_y)
				y_enemy_pos += MAX_VELOCITY;
		}

		enemy_box = {x_enemy_pos, y_enemy_pos, BOX_WIDTH, BOX_HEIGHT};
		//Enemy box is within 8 pixels of an obstacle
		if (check_vicinity(&enemy_box, &obst)) {
			//correct by going down a pixel
			if (dir) {
				x_enemy_pos += MAX_VELOCITY;
				y_enemy_pos += 2;
			}
			//correct by going up a pixel
			else {
				x_enemy_pos += -MAX_VELOCITY;
				y_enemy_pos -= 2;
			}
		}
		else if (check_vicinity(&enemy_box, &obst_1)) {
			//correct by going down a pixel
			if (dir) {
				x_enemy_pos += MAX_VELOCITY;
				y_enemy_pos += 2;
			}
			//correct by going up a pixel
			else {
				x_enemy_pos += -MAX_VELOCITY;
				y_enemy_pos -= 2;
			}
		}
		else if (check_vicinity(&enemy_box, &obst_2)) {
			//correct by going down a pixel
			if (dir) {
				x_enemy_pos += MAX_VELOCITY;
				y_enemy_pos += 2;
			}
			//correct by going up a pixel
			else {
				x_enemy_pos += -MAX_VELOCITY;
				y_enemy_pos -= 2;
			}
		}
		//Set direction
		if (y_enemy_pos == 60 && x_enemy_pos == 75)
			dir = false;
		if (y_enemy_pos == enemy_start_y && x_enemy_pos == enemy_start_x)
			dir = true;

		//Ensure enemy doesn't go offscreen
		if(x_enemy_pos > SCREEN_WIDTH - 2*BOX_WIDTH) {
			x_enemy_pos = SCREEN_WIDTH - 2*BOX_WIDTH;
		}
		if(x_enemy_pos < 20){
			x_enemy_pos = 20;
		}
		if(y_enemy_pos < 20){
			y_enemy_pos = 20;
		}
		if(y_enemy_pos > SCREEN_HEIGHT - 2*BOX_HEIGHT) {
			y_enemy_pos = SCREEN_HEIGHT - 2*BOX_HEIGHT;
		}

		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(gRenderer);

		// Render array of tiles
		for (int x = BORDER_GAP + TILE_SIZE, i = 0; x < SCREEN_WIDTH - BORDER_GAP - TILE_SIZE; x+=TILE_SIZE, i++) {
			for (int y = TILE_SIZE, j = 0; y < SCREEN_HEIGHT - TILE_SIZE; y+=TILE_SIZE, j++) {
				cur_out = { x, y, TILE_SIZE, TILE_SIZE};
				SDL_RenderCopy(gRenderer, gTileSheet, &gTileRects[tile_map[i][j]], &cur_out);
			}
		}

		//FROM BORDERGAP + TILE SIZE TO GET INTERIOR OF MAP
		//x represents the pixels of the screen, not the tile index anymore
		for (int x = BORDER_GAP + TILE_SIZE; x < SCREEN_WIDTH - BORDER_GAP - TILE_SIZE; x+=TILE_SIZE) {
			for (int y = TILE_SIZE; y < SCREEN_HEIGHT - TILE_SIZE; y+=TILE_SIZE) {
				cur_out = { x, y, TILE_SIZE, TILE_SIZE};
				SDL_RenderCopy(gRenderer, gTileSheet, &gTileRects[0], &cur_out);
			}
		}

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
		//Render player box
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0xFF, 0xFF);
		SDL_Rect fillRect = {x_pos, y_pos, BOX_WIDTH, BOX_HEIGHT};
		SDL_RenderCopy(gRenderer, gTank_Blue, NULL, &fillRect);
		//Render obstacle 1
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0xFF, 0xFF);
		SDL_RenderFillRect(gRenderer, &obst);
		//Render obstacle 2
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
		SDL_RenderFillRect(gRenderer, &obst_1);
		//Render obstacle 3
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
		SDL_RenderFillRect(gRenderer, &obst_2);
		//Render enemy box
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_Rect enemy_box = {x_enemy_pos, y_enemy_pos, BOX_WIDTH, BOX_HEIGHT};
		//SDL_RenderFillRect(gRenderer, &enemy_box);
		SDL_RenderCopy(gRenderer, gTank_Red, NULL, &enemy_box);

		SDL_RenderPresent(gRenderer);
	} //end of game loop

	close();
}
