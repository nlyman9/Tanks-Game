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
constexpr int BOX_WIDTH = 20;
constexpr int BOX_HEIGHT = 20;
constexpr int BULLET_WIDTH = 4;
constexpr int BULLET_HEIGHT = 6;
constexpr int MAX_VELOCITY = 1;
constexpr int BULLET_VELOCITY = 2;

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
bool left = true;
bool bullet_fire[3] = {false, false, false};
bool enemy_bullet_fire = false;
int** tile_map;
int** move_map;
std::vector<SDL_Texture*> gTex;
SDL_Rect cur_out;
SDL_Rect* tileArray;


struct coordinate {
	int row;
	int col;
	int weight;
};

void printTileMap(int** map){
	printf("    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3\n    X X X X X X X X X X X X X X X X X X X X X X X X\n");
	for(int i = 0; i < 13; i++){
		printf("%d X ", (i%10));
		for(int j = 0; j < 24; j++) {

			if(map[j][i] == 2){
				printf("X ");
			}
			else if(map[j][i] == 100){
				printf("F ");
			}
			else if(map[j][i] == 101){
				printf("S ");
			}
			else{
				printf("_ ");
			}

		}
		printf("\n");
	}
}

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

	printTileMap(tile_map);

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
	if (a->y + a->h <= b->y - 2)
		return false;
	if (a->y >= b->y + b->h + 2)
		return false;

	// Check horizontal overlap
	if (a->x >= b->x + b->w + 2)
		return false;
	if (a->x + a->w <= b->x - 2)
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

int xArrPosL(int pos){
	int lowBound = TILE_SIZE-20;
	int upBound = TILE_SIZE*2-20;
	for(int i = 0; i < 24; i++){
		if(pos <= upBound && pos >= lowBound){
			return i;
		}
		lowBound += TILE_SIZE;
		upBound += TILE_SIZE;
	}
	return 23;
}

int xArrPosR(int pos){
	int lowBound = TILE_SIZE+25;
	int upBound = TILE_SIZE*2+25;
	for(int i = 0; i < 24; i++){
		if(pos <= upBound && pos >= lowBound){
			return i;
		}
		lowBound += TILE_SIZE;
		upBound += TILE_SIZE;
	}
	return 23;
}

int yArrPos(int pos){
	int lowBound = TILE_SIZE-30;
	int upBound = TILE_SIZE*2-30;
	for(int i = 0; i < 12; i++){
		if(pos <= upBound && pos >= lowBound){
			return i - 1;
		}
		lowBound += TILE_SIZE;
		upBound += TILE_SIZE;
	}
	return 11;

}

//x block 0 - 23
int findXblock(int pos){
	int center = pos + BOX_WIDTH/2;
	int trueX = center - TILE_SIZE - 16;
	int block = trueX / TILE_SIZE;
	return block;
}

//y block 0 - 12
int findYBlock(int pos){
	int center = pos + BOX_HEIGHT/2;
	int trueY = center - TILE_SIZE;
	int block = trueY / TILE_SIZE;
	return block;
}


int** generateMoveMap(int** tile_map, SDL_Rect player, SDL_Rect enemy){
	int** new_map;
	new_map = new int*[24];

	for(int j = 0; j < 24; j++)
	{
		new_map[j] = new int[13];
		for(int h = 0; h < 13; h++)
		{
			new_map[j][h] = tile_map[j][h];
		}
	}

	int xPlayer = findXblock(player.x);
	int xEnemy = findXblock(enemy.x);
	int yPlayer = findYBlock(player.y);
	int yEnemy = findYBlock(enemy.y);

	printf("Player coordinates: %d, %d\n", yPlayer, xPlayer);
	printf("Enemy coordinates: %d, %d\n", yEnemy, xEnemy);

	new_map[xPlayer][yPlayer] = 100;
	new_map[xEnemy][yEnemy] = 101;

	printTileMap(new_map);
	return new_map;
}

bool validMove(coordinate moveTo, coordinate currentlyAt){
	int moveToRow = moveTo.row;
	int moveToCol = moveTo.col;
	int moveToWeight = moveTo.weight;
	int atRow = currentlyAt.row;
	int atCol = currentlyAt.col;
	int atWeight = currentlyAt.weight;

	//if weights valid
	if(moveToWeight == (atWeight - 1)){
		//if same row different column
		if((moveToRow == atRow) && (moveToCol == (atCol - 1) || (moveToCol == (atCol + 1)))){
			return true;
		}
		//if same col different row
		if((moveToCol == atCol) && (moveToRow == (atRow - 1) || (moveToRow == (atRow + 1)))){
			return true;
		}
	}
	return false;
}

std::vector<coordinate> generatePath(int** move_map, SDL_Rect player, SDL_Rect enemy){
	int xPlayer = findXblock(player.x);
	int yPlayer = findYBlock(player.y);
	printf("XPlayer = %d, YPlayer = %d\n", xPlayer, yPlayer);
	std::vector<coordinate> coordList;
	std::vector<coordinate> finalPath;
	coordinate enemyStart = {findYBlock(enemy.y), findXblock(enemy.x), 0};
	coordList.push_back(enemyStart);
	bool keepGoing = false;
	bool inList = false;
	int count = 0;
	int coordListLength;

	while(!keepGoing) {
	//add surrounding squares to list
		coordListLength = coordList.size();
		//printf("List length = %d\n", coordListLength);
		for(int i = 0; i < coordListLength; i++){
			//printf("Current coordinate = {%d, %d, %d}\n", coordList[i].row, coordList[i].col, coordList[i].weight);
			if(coordList[i].weight == count){
				coordinate left = {coordList[i].row, coordList[i].col - 1, coordList[i].weight + 1};
				//check isnt a wall
				if(!(left.row < 0 || left.row > 12 || left.col < 0 || left.col > 23) && (move_map[left.col][left.row] != 2)){
					//check isnt already in list
					for(int j = 0; j < coordListLength; j++){
						if(left.row == coordList[j].row && left.col == coordList[j].col){
							inList = true;
							break;
						}
					}
					if(!inList){
						coordList.push_back(left);
						//printf("LEFT {%d, %d, %d}\n", left.row, left.col, left.weight);
						if(left.row == yPlayer && left.col == xPlayer){
							keepGoing = true;
						}
					}
					inList = false;
				}
				coordinate right = {coordList[i].row, coordList[i].col + 1, coordList[i].weight + 1};
				if(!(right.row < 0 || right.row > 12 || right.col < 0 || right.col > 23) && (move_map[right.col][right.row] != 2)){
					for(int j = 0; j < coordListLength; j++){
						if(right.row == coordList[j].row && right.col == coordList[j].col){
							inList = true;
							break;
						}
					}
					if(!inList){
						coordList.push_back(right);
						//printf("RIGHT {%d, %d, %d}\n", right.row, right.col, right.weight);
						if(right.row == yPlayer && right.col == xPlayer){
							keepGoing = true;
						}
					}
					inList = false;
				}
				coordinate up = {coordList[i].row - 1, coordList[i].col, coordList[i].weight + 1};
				if(!(up.row < 0 || up.row > 12 || up.col < 0 || up.col > 23) && (move_map[up.col][up.row] != 2)){
					for(int j = 0; j < coordListLength; j++){
						if(up.row == coordList[j].row && up.col == coordList[j].col){
							inList = true;
							break;
						}
					}
					if(!inList){
						coordList.push_back(up);
						//printf("UP {%d, %d, %d}\n", up.row, up.col, up.weight);
						if(up.row == yPlayer && up.col == xPlayer){
							keepGoing = true;
						}
					}
					inList = false;
				}
				coordinate down = {coordList[i].row + 1, coordList[i].col, coordList[i].weight + 1};
				if(!(down.row < 0 || down.row > 12 || down.col < 0 || down.col > 23) && (move_map[down.col][down.row] != 2)){
					for(int j = 0; j < coordListLength; j++){
						if(down.row == coordList[j].row && down.col == coordList[j].col){
							inList = true;
							break;
						}
					}
					if(!inList){
						coordList.push_back(down);
						//printf("DOWN {%d, %d, %d}\n", down.row, down.col, down.weight);
						if(down.row == yPlayer && down.col == xPlayer){
							keepGoing = true;
						}
					}
					inList = false;
				}
			}
		}
		count++;
	}

	coordinate currentCoord = {yPlayer, xPlayer, count};
	finalPath.push_back(currentCoord);
	coordListLength = coordList.size();

	while(count != 0){
		for(int i = 0; i < coordListLength; i++){
			if(coordList[i].weight == (count - 1)){
				if(validMove(coordList[i], currentCoord)){
					finalPath.push_back(coordList[i]);
					currentCoord = coordList[i];
					count--;
					break;
				}
			}
		}
	}
	/*
	for(int i = 0; i < coordList.size(); i++){
		printf("{%d, %d, %d}", coordList[i].row, coordList[i].col, coordList[i].weight);
	}*/

	for(int i = 0; i < finalPath.size(); i++){
		printf("{%d, %d, %d}", finalPath[i].row, finalPath[i].col, finalPath[i].weight);
	}

	printf("\n");

	return finalPath;
}

bool shouldShoot(int x_player, int x_enemy, int y_player, int y_enemy){
	int cannon = x_enemy - BOX_WIDTH/2;
	int x_player_rightEdge = x_player + BOX_WIDTH;
	if(cannon <= x_player_rightEdge && cannon >= x_player && y_player < y_enemy){
		return true;
	}
	return false;
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

	tileArray = new SDL_Rect[312];
	int count = 0;
	for (int x = BORDER_GAP + TILE_SIZE, i = 0; x < SCREEN_WIDTH - BORDER_GAP - TILE_SIZE; x+=TILE_SIZE, i++) {
		for (int y = TILE_SIZE, j = 0; y < SCREEN_HEIGHT - TILE_SIZE; y+=TILE_SIZE, j++) {
			cur_out = { x, y, TILE_SIZE, TILE_SIZE};
			if(tile_map[i][j] == 2){
				tileArray[count++] = cur_out;
			}
		}
	}

	SDL_Event e;
	bool gameon = true;
	int c;
	SDL_Rect cur_out;

	// Current position to render the box
	// Start off with it in the middle
	int x_pos = SCREEN_WIDTH - BOX_WIDTH/2 - 470;
	int y_pos = SCREEN_HEIGHT - BOX_HEIGHT/2 - 54;
	//int x_pos = 55;
	//int y_pos = 60;
	//Enemy box start position
	int x_enemy_pos = SCREEN_WIDTH - BOX_WIDTH/2 - 275;
	int y_enemy_pos = SCREEN_HEIGHT - BOX_HEIGHT/2 - 267;
	int enemy_start_x = x_enemy_pos;
	//int enemy_start_y = y_enemy_pos;
	//Initialize enemy box
	SDL_Rect enemy_box = {x_enemy_pos, y_enemy_pos, BOX_WIDTH, BOX_HEIGHT};
	SDL_Rect player_box = {x_pos, y_pos, BOX_WIDTH, BOX_HEIGHT};

	//move_map = generateMoveMap(tile_map, player_box, enemy_box);

	std::vector<coordinate> enemyPath = generatePath(tile_map, player_box, enemy_box);

	// Current velocity of the box
	// Start off at reset
	int x_vel = 0;
	int y_vel = 0;

	int x_bullet_pos[3];
	int y_bullet_pos[3];

	int x_enemy_bullet_pos;
	int y_enemy_bullet_pos;

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

					case SDLK_SPACE:
						for(int i = 0; i < 3; i++){
							if(!bullet_fire[i]){
								x_bullet_pos[i] = x_pos + BOX_WIDTH/2;
								y_bullet_pos[i] = y_pos;
								bullet_fire[i] = true;
								break;
							}
						}
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

		if(shouldShoot(x_pos, x_enemy_pos, y_pos, y_enemy_pos) && !enemy_bullet_fire){
			printf("enemy fired\n");
			enemy_bullet_fire = true;
			x_enemy_bullet_pos = x_enemy_pos + BOX_WIDTH/2;
			y_enemy_bullet_pos = y_enemy_pos;
		}

		if(enemy_bullet_fire){
			y_enemy_bullet_pos -= BULLET_VELOCITY;
		}

		if(enemy_bullet_fire){
			SDL_Rect bullet_collide = {x_enemy_bullet_pos, y_enemy_bullet_pos, BULLET_WIDTH, BULLET_HEIGHT};
			for(int j = 0; j < 312; j++){
				cur_out = tileArray[j];
				if(player_vicinity(&bullet_collide, &cur_out)){
					enemy_bullet_fire = false;
				}
			}
			if(y_enemy_bullet_pos == TILE_SIZE){
				enemy_bullet_fire = false;
			}
		}

		for(int i = 0; i < 3; i++){
			if(bullet_fire[i]){
				if(y_bullet_pos[i] == TILE_SIZE){
						bullet_fire[i] = false;
				}
				else{
					y_bullet_pos[i] -= BULLET_VELOCITY;
				}
			}
		}

		for(int i = 0; i < 3; i++){
			if(bullet_fire[i]){
				SDL_Rect bullet_collide = {x_bullet_pos[i], y_bullet_pos[i], BULLET_WIDTH, BULLET_HEIGHT};
				for(int j = 0; j < 312; j++){
					cur_out = tileArray[j];
					if(player_vicinity(&bullet_collide, &cur_out)){
						bullet_fire[i] = false;
					}
				}
			}
		}

		for(int i = 0; i < 3; i++){
			if(bullet_fire[i]){
				SDL_Rect bullet_collide = {x_bullet_pos[i], y_bullet_pos[i], BULLET_WIDTH, BULLET_HEIGHT};
				if(player_vicinity(&bullet_collide, &enemy_box)){
					printf("YOU WIN\n");
					bullet_fire[i] = false;
				}
			}
		}


		if(enemy_bullet_fire){
			SDL_Rect enemy_bullet_collide = {x_enemy_bullet_pos, y_enemy_bullet_pos, BULLET_WIDTH, BULLET_HEIGHT};
			if(player_vicinity(&enemy_bullet_collide, &player_box)){
				printf("ENEMY WINS\n");
				enemy_bullet_fire = false;
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

		player_box = {x_pos, y_pos, BOX_WIDTH, BOX_HEIGHT};
		for(int i = 0; i < 312; i++){
			cur_out = tileArray[i];
			if(player_vicinity(&player_box, &cur_out)){
				x_pos -= x_vel;
				y_pos -= y_vel;
			}
		}

		//printf("In Block %d, %d\n", findYBlock(y_pos), findXblock(x_pos));

		//Prevent the box from going offscreen
		if(x_pos > SCREEN_WIDTH - 2*BOX_WIDTH - TILE_SIZE) {
			x_pos = SCREEN_WIDTH - 2*BOX_WIDTH - TILE_SIZE;
		}
		if(x_pos < TILE_SIZE + BOX_WIDTH){
			x_pos = TILE_SIZE + BOX_WIDTH;
		}
		if(y_pos < TILE_SIZE){
			y_pos = TILE_SIZE;
		}
		if(y_pos > SCREEN_HEIGHT - BOX_HEIGHT - TILE_SIZE) {
			y_pos = SCREEN_HEIGHT - BOX_HEIGHT - TILE_SIZE;
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
/*
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
		for(int i = 0; i < 312; i++){
			cur_out = tileArray[i];
			if(check_vicinity(&enemy_box, &cur_out)){
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
		}

		if(left) {
			if(tile_map[xArrPosL(x_enemy_pos)][yArrPos(y_enemy_pos)] == 2){
				x_enemy_pos -= MAX_VELOCITY;
			}
			else{
				y_enemy_pos -= MAX_VELOCITY;
			}

		}
		else{
			if(tile_map[xArrPosR(x_enemy_pos)][yArrPos(y_enemy_pos)] == 2){
				x_enemy_pos += MAX_VELOCITY;
			}
			else{
				y_enemy_pos -= MAX_VELOCITY;
			}
		}*/

		if(enemyPath.size() > 1){

			coordinate moveFrom = enemyPath[enemyPath.size() - 1];
			coordinate moveTo = enemyPath[enemyPath.size() - 2];

			//move LEFT
			if(moveFrom.col > moveTo.col){
				x_enemy_pos -= MAX_VELOCITY;
				y_enemy_pos += 0;
				if(x_enemy_pos < (moveTo.col * TILE_SIZE + TILE_SIZE + 36)){
					enemyPath.pop_back();
				}
			}
			//move right
			if(moveFrom.col < moveTo.col){
				x_enemy_pos += MAX_VELOCITY;
				y_enemy_pos += 0;
				if(x_enemy_pos > (moveTo.col * TILE_SIZE + TILE_SIZE*2 - 20)){
					enemyPath.pop_back();
				}
			}
			//move up
			if(moveFrom.row > moveTo.row){
				x_enemy_pos += 0;
				y_enemy_pos -= MAX_VELOCITY;
				if(y_enemy_pos < (moveTo.row * TILE_SIZE + TILE_SIZE + 20)){
					enemyPath.pop_back();
				}
			}
			//move DOWN
			if(moveFrom.row < moveTo.row){
				x_enemy_pos += 0;
				y_enemy_pos += MAX_VELOCITY;
				if(y_enemy_pos > (moveTo.row * TILE_SIZE + TILE_SIZE + 20)){
					enemyPath.pop_back();
				}
			}
		}
	/*
		if(destinationReached(x_enemy_pos, y_enemy_pos, moveTo)){
			enemyPath.pop_back();
			printf("Move From: {%d, %d}\n", moveFrom.row, moveFrom.col);
			printf("Move To:   {%d, %d}\n", moveTo.row, moveTo.col);
		}
*/

		if(x_enemy_pos == 75){
			left = false;
		}
		if(x_enemy_pos == enemy_start_x){
			left = true;
		}

		enemy_box = {x_enemy_pos, y_enemy_pos, BOX_WIDTH, BOX_HEIGHT};
		//Enemy box is within 8 pixels of an obstacle
		for(int i = 0; i < 312; i++){
			cur_out = tileArray[i];
			if(check_vicinity(&enemy_box, &cur_out)){
				if (left) {
					//x_enemy_pos += MAX_VELOCITY;
					y_enemy_pos += 3;
				}
				//correct by going up a pixel
				else {
					//x_enemy_pos += -MAX_VELOCITY;
					y_enemy_pos += 3;
				}
			}
		}
/*
		//Set direction
		if (y_enemy_pos == 60 && x_enemy_pos == 75)
			dir = false;
		if (y_enemy_pos == enemy_start_y && x_enemy_pos == enemy_start_x)
			dir = true;
*/
		//Ensure enemy doesn't go offscreen
		if(x_enemy_pos > SCREEN_WIDTH - 2*BOX_WIDTH) {
			x_enemy_pos = SCREEN_WIDTH - 2*BOX_WIDTH;
		}
		if(x_enemy_pos < TILE_SIZE){
			x_enemy_pos = TILE_SIZE;
		}
		if(y_enemy_pos < TILE_SIZE){
			y_enemy_pos = TILE_SIZE;
		}
		if(y_enemy_pos > SCREEN_HEIGHT - 2*BOX_HEIGHT) {
			y_enemy_pos = SCREEN_HEIGHT - 2*BOX_HEIGHT;
		}

		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(gRenderer);

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

		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_Rect enemy_box = {x_enemy_pos, y_enemy_pos, BOX_WIDTH, BOX_HEIGHT};
		//SDL_RenderFillRect(gRenderer, &enemy_box);
		SDL_RenderCopy(gRenderer, gTank_Red, NULL, &enemy_box);

		for(int i = 0; i < 3; i++){
			if(bullet_fire[i]){
				SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xff, 0xff);
				SDL_Rect bullet = {x_bullet_pos[i], y_bullet_pos[i], BULLET_WIDTH, BULLET_HEIGHT};
				SDL_RenderFillRect(gRenderer, &bullet);
			}
		}

		if(enemy_bullet_fire){
			SDL_SetRenderDrawColor(gRenderer, 0xff, 0x00, 0x00, 0xff);
			SDL_Rect enemy_bullet = {x_enemy_bullet_pos, y_enemy_bullet_pos, BULLET_WIDTH, BULLET_HEIGHT};
			SDL_RenderFillRect(gRenderer, &enemy_bullet);
		}
		SDL_RenderPresent(gRenderer);
	} //end of game loop

	close();
}
