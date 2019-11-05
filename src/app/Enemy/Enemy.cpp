#include "Constants.hpp"
#include "Enemy.hpp"

// Enemy::Enemy(Sprite sprite, Sprite turret, int x, int y) : sprite{sprite}, turret{turret}, x_enemy_pos{x}, y_enemy_pos{y} {}

Enemy::Enemy(float x, float y, Player* player) :  x_enemy_pos{x}, y_enemy_pos{y}, gPlayer{player} {}

Enemy::~Enemy() {}


/**
 * @brief draws the Enemy object
 *  Overrides base class OBJECT
 *
 * @param update_lag - the value to extrapolate by
 */
void Enemy::draw(SDL_Renderer *gRenderer, double update_lag) {

  // Extrapolate the x and y positions
  // "Solves" stuck in the middle rendering.
  // TODO change MAX_VELOCITY to the enemy's velocity

  // int x_pos = getX() + x_velocity * update_lag;
  // int y_pos = getY() + y_velocity * update_lag;

  // Render enemy
  // SDL_Rect src = {0, 0, 48, 48};
  // SDL_Rect dst = {x_enemy_pos, y_enemy_pos, 39, 48};
  // SDL_RenderCopyEx(gRenderer, getSprite()->getTexture(), &src, &dst, 0, NULL, SDL_FLIP_NONE);
  SDL_Rect pos = {(int)x_enemy_pos, (int)y_enemy_pos, TANK_WIDTH, TANK_HEIGHT};
  SDL_RenderCopy(gRenderer, getSprite()->getTexture(), NULL, &pos);
}

/**
 * @brief update the Enemy object
 *  Overrides base class OBJECT
 *
 */
void Enemy::update() {
  updatePos();
}

/**
 * @brief move the Enemy an offset from its current x-y position
 *  Overrides base class OBJECT
 *
 * @param x - how much to move Enemy's current x position by
 * @param y - how much to move Enemy's current y position by
 *
 * @return true  - moved Enemy succesfully
 * @return false - failed to move Enemy
 */
bool Enemy::move(float x, float y) {
    return false;
}

/**
 * @brief place/teleport Enemy to a specified x-y location
 *
 * @param x - set x position
 * @param y - set y position
 *
 * @return true  - placed Enemy succesfully
 * @return false - failed to place Enemy
 */
bool Enemy::place(float x, float y) {
    return false;
}

/* Enemy Specific Functions */

bool Enemy::fire() {
    return false;
}

bool Enemy::rotateEnemy(float theta) {
    return false;
}

bool Enemy::rotateTurret(float theta) {
    return false;
}

bool Enemy::checkPos(float playX, float playY, float enemX, float enemY) {
  double stepOne = (double)(pow((playX - enemX), 2) + pow((playY - enemY), 2));

	double distanceAway = (pow(stepOne, .5));

	if (distanceAway < 200.0)
	{
		return true;
	}
	return false;
}

bool Enemy::checkWall(float x, float y) {
  //left wall
	if (x <= TILE_SIZE + BORDER_GAP)
	{
		return true;
	}
	//right wall
	else if (x >= SCREEN_WIDTH - BORDER_GAP * TANK_WIDTH)
	{
		return true;
	}
	//top wall
	else if (y <= TILE_SIZE)
	{
		return true;
	}
	//bottom wall
	else if (y >= SCREEN_HEIGHT - 2 * TANK_HEIGHT)
	{
		return true;
	}
	else
	{
		return false;
	}
}

float Enemy::getX(){
  return x_enemy_pos;
}

float Enemy::getY(){
  return y_enemy_pos;
}

void Enemy::updatePos() {
  if(enemyPath.size() < 2){
    setPathway(this->tile_map, *this->gPlayer, *this);
  }
  float x_pos = gPlayer->getX();
  float y_pos = gPlayer->getY();
  bool retreat;
  retreat = checkPos(x_pos, y_pos, x_enemy_pos, y_enemy_pos);
  //See if player and enemy are intersecting
  SDL_Rect enemy_rect;
  SDL_Rect player_rect;
  enemy_rect = {(int)x_enemy_pos, (int)y_enemy_pos, TANK_WIDTH, TANK_HEIGHT};
  player_rect = {(int)x_pos, (int)y_pos, TANK_WIDTH, TANK_HEIGHT};
  //std::cout << enemy_rect.w << ":" << enemy_rect.h << ":" << enemy_rect.x << ":" << enemy_rect.y << std::endl;

  SDL_Rect* overlap;
  overlap = check_collision(&enemy_rect, &player_rect);

  /*
  SDL_Rect enemy_rect;
  SDL_Rect player_rect;
  SDL_Rect* overlap;
  enemy_rect = {x_enemy_pos, y_enemy_pos, TANK_WIDTH, TANK_HEIGHT};
  player_rect = {x_pos, y_pos, TANK_WIDTH, TANK_HEIGHT};
  overlap = check_collision(&enemy_rect, &player_rect);
  */
  bool nearWall;
  nearWall = checkWall(x_enemy_pos, y_enemy_pos);

  int startingPosition =  SCREEN_WIDTH - TANK_WIDTH/2 - 75;

  if(enemyPath.size() > 1){

    coordinate moveFrom = enemyPath[enemyPath.size() - 1];
    coordinate moveTo = enemyPath[enemyPath.size() - 2];

    //move LEFT
    if(moveFrom.col > moveTo.col){
      //std::cout << "Moving left" << std::endl;
      x_enemy_pos -= MAX_VELOCITY;
      y_enemy_pos += 0;
      if(x_enemy_pos < (moveTo.col * TILE_SIZE + TILE_SIZE + 36)){
        enemyPath.pop_back();
      }
      //check collision with player
      if (overlap != nullptr) {
        x_enemy_pos += MAX_VELOCITY;
        //std::cout << overlap->w << ":" << overlap->h << ":" << overlap->x << ":" << overlap->y << std::endl;
      }
    }
    //move right
    if(moveFrom.col < moveTo.col){
      //std::cout << "Moving right" << std::endl;
      x_enemy_pos += MAX_VELOCITY;
      y_enemy_pos += 0;
      if(x_enemy_pos > (moveTo.col * TILE_SIZE + TILE_SIZE*2 - 20)){
        enemyPath.pop_back();
      }
      //check collision
      if (overlap != nullptr) {
        x_enemy_pos -= MAX_VELOCITY;
        //std::cout << overlap->w << ":" << overlap->h << ":" << overlap->x << ":" << overlap->y << std::endl;
      }
    }
    //move up
    if(moveFrom.row > moveTo.row){
      x_enemy_pos += 0;
      y_enemy_pos -= MAX_VELOCITY;
      if(y_enemy_pos < (moveTo.row * TILE_SIZE + TILE_SIZE + 20)){
        enemyPath.pop_back();
      }
      //check collision
      if (overlap != nullptr) {
        y_enemy_pos += MAX_VELOCITY;
        //std::cout << overlap->w << ":" << overlap->h << ":" << overlap->x << ":" << overlap->y << std::endl;
      }
    }
    //move DOWN
    if(moveFrom.row < moveTo.row){
      x_enemy_pos += 0;
      y_enemy_pos += MAX_VELOCITY;
      if(y_enemy_pos > (moveTo.row * TILE_SIZE + TILE_SIZE + 20)){
        enemyPath.pop_back();
      }
      //check collision
      if (overlap != nullptr) {
        y_enemy_pos -= MAX_VELOCITY;
        //std::cout << overlap->w << ":" << overlap->h << ":" << overlap->x << ":" << overlap->y << std::endl;
      }
    }
  }

  SDL_Rect* box = get_box(); // required to update box    //Enemy box is within 8 pixels of an obstacle
  /*
  SDL_Rect player_rect = {x_pos, y_pos, TANK_WIDTH, TANK_HEIGHT};
  overlap = check_collision(&currentPos, &player_rect);
  if (overlap != nullptr) {
    setPos(getX() - (x_vel * updateStep), getY() - (y_vel * updateStep));
  }
  */

  for(auto obstacle : obstacles) {
    if(check_collision(&obstacle)) {
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


  if(x_enemy_pos > SCREEN_WIDTH - 2*TANK_WIDTH) {
    x_enemy_pos = SCREEN_WIDTH - 2*TANK_WIDTH;
  }
  if(x_enemy_pos < TILE_SIZE){
    x_enemy_pos = TILE_SIZE;
  }
  if(y_enemy_pos < TILE_SIZE){
    y_enemy_pos = TILE_SIZE;
  }
  if(y_enemy_pos > SCREEN_HEIGHT - 2*TANK_HEIGHT) {
    y_enemy_pos = SCREEN_HEIGHT - 2*TANK_HEIGHT;
  }
}


int Enemy::xArrPosR(float pos) {
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

int Enemy::yArrPos(float pos) {
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

int Enemy::xArrPosL(float pos) {
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

void Enemy::setTileMap(std::vector<std::vector<int>>* tileMap) {
	tile_map = *tileMap;
}

/** TODO Finish implementation once theta is added to enemy.
 * @brief Get the bounding box of the player's tank
 *
 * The bounding box is based off 4 points: backLeft, backRight, frontLeft, frontRight;
 *  - The front of the tank is the direction the tank is pointing. The back is the opposit.
 *  - The left side is the lefthand side of the direction the the tank is facing. The right is the right hand side.
 *
 * @return BoundingBox*
 */
BoundingBox* Enemy::getBoundingBox() {
    BoundingBox *box = new BoundingBox();
    float sqrt_2 = 1.414213;
    float radius = (TANK_WIDTH * sqrt_2)/2;

    // The player's theta increases clockwise instead of how trig functions naturally increases counter-clockwise
    // So, the the trig functions for backLeft use theta+225 instead of theta+135
    box->backLeft.x = (getX() + (TANK_WIDTH/2));// + radius * cos((theta+225) * M_PI / 180);
    box->backLeft.y = (getY() + (TANK_HEIGHT/2));// + radius * sin((theta+225) * M_PI / 180);

    box->backRight.x = (getX() + (TANK_WIDTH/2));// + radius * cos((theta+135) * M_PI / 180);
    box->backRight.y = (getY() + (TANK_HEIGHT/2));// + radius * sin((theta+135) * M_PI / 180);

    box->frontLeft.x = (getX() + (TANK_WIDTH/2));// + radius * cos((theta+315) * M_PI / 180);
    box->frontLeft.y = (getY() + (TANK_HEIGHT/2));// + radius * sin((theta+315) * M_PI / 180);

    box->frontRight.x = (getX() + (TANK_WIDTH/2));// + radius * cos((theta+45) * M_PI / 180);
    box->frontRight.y = (getY() + (TANK_HEIGHT/2));//+ radius * sin((theta+45) * M_PI / 180);

    return box;
}
void Enemy::setPathway(std::vector<std::vector<int>> move_map, Player player, Enemy enemy){
  int ghostXblock = findXBlock(player.getX());
  int ghostYblock = findYBlock(player.getY());
  int enemyXblock = findXBlock(enemy.getX());
  int enemyYblock = findYBlock(enemy.getY());
  if(abs(ghostXblock - enemyXblock) < 2 && abs(ghostYblock - enemyYblock) < 2){
    enemyPath = generatePath(move_map, player, enemy);
  }
  else{
    coordinate newGhostPos = Enemy::newGhostPos(ghostXblock, ghostYblock, enemyXblock, enemyYblock);
    int ghostX = newGhostPos.col * TILE_SIZE + TILE_SIZE + BORDER_GAP;
    int ghostY = newGhostPos.row * TILE_SIZE + TILE_SIZE;
    Player* ghost = new Player(ghostX, ghostY);
    enemyPath = generatePath(move_map, *ghost, enemy);
  }
}

coordinate Enemy::newGhostPos(int gX, int gY, int eX, int eY){
  coordinate newPos = {eY, eX, 0};
  if(gX == eX && gY == eY){
    return newPos;
  }
  //player above enemy
  if(gY <= eY){
    //player left of enemy
    if(gX <= eX){
      //move up and to the left
      newPos.row -= 2;
      newPos.col -= 2;
    }
    else{
      //move up and to the right
      newPos.row -= 2;
      newPos.col += 2;
    }
  }
  else{
    if(gX <= eX){
      //move down and to the left
      newPos.row += 2;
      newPos.col -= 2;
    }
    else{
      //move down and to the right
      newPos.row += 2;
      newPos.col += 2;
    }
  }
  if(!isValidBlock(newPos.row, newPos.col)){
    newPos = findClosestOpenBlock(newPos);
  }
  return newPos;
}

coordinate Enemy::findClosestOpenBlock(coordinate start){
  coordinate test = {start.row, start.col, 0};
  int count = 0;
  int increment = 1;
  while(1){
    //check above
    if(count%8 == 0){
      test.row -= increment;
      //printf("Target block = X: %d, Y: %d\n", test.row, test.col);
      if(isValidBlock(test.row, test.col)){
        return test;
      }
      test.row += increment;
    }
    //check above & right
    else if(count%8 == 1){
      test.row -= increment;
      test.col += increment;
      //printf("Target block = X: %d, Y: %d\n", test.row, test.col);
      if(isValidBlock(test.row, test.col)){
        return test;
      }
      test.row += increment;
      test.col -= increment;
    }
    //check right
    else if(count%8 == 2){
      test.col += increment;
      //printf("Target block = X: %d, Y: %d\n", test.row, test.col);
      if(isValidBlock(test.row, test.col)){
        return test;
      }
      test.col -= increment;
    }
    //check below & right
    else if(count%8 == 3){
      test.row += increment;
      test.col += increment;
      //printf("Target block = X: %d, Y: %d\n", test.row, test.col);
      if(isValidBlock(test.row, test.col)){
        return test;
      }
      test.row -= increment;
      test.col -= increment;
    }
    //check below
    else if(count%8 == 4){
      test.row += increment;
      //printf("Target block = X: %d, Y: %d\n", test.row, test.col);
      if(isValidBlock(test.row, test.col)){
        return test;
      }
      test.row -= increment;
    }
    //check below & left
    else if(count%8 == 5){
      test.row += increment;
      test.col -= increment;
      //printf("Target block = X: %d, Y: %d\n", test.row, test.col);
      if(isValidBlock(test.row, test.col)){
        return test;
      }
      test.row -= increment;
      test.col += increment;
    }
    //check left
    else if(count%8 == 6){
      test.col -= increment;
      //printf("Target block = X: %d, Y: %d\n", test.row, test.col);
      if(isValidBlock(test.row, test.col)){
        return test;
      }
      test.col += increment;
    }
    //check above & left
    else {
      test.row -= increment;
      test.col -= increment;
      //printf("Target block = X: %d, Y: %d\n", test.row, test.col);
      if(isValidBlock(test.row, test.col)){
        return test;
      }
      test.row += increment;
      test.col += increment;
      increment++;
    }
    count++;
  }
}

bool Enemy::isValidBlock(int x, int y){
  int curX = findXBlock(x_enemy_pos);
  int curY = findYBlock(y_enemy_pos);
  //printf("Target Loc X: %d, Y; %d, CURRENT ENEMY X: %d, Y: %d\n", x, y, curX, curY);
  if(y < 0 || y > 23 || x < 0 || x > 12){
    return false;
  }
  if((tile_map[y][x] == 0) && !(y == curX && x == curY)){
    return true;
  }
  return false;
}

bool Enemy::validMove(coordinate moveTo, coordinate currentlyAt){
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

std::vector<coordinate> Enemy::generatePath(std::vector<std::vector<int>> move_map, Player player, Enemy enemy){
	int xPlayer = findXBlock(player.getX());
	int yPlayer = findYBlock(player.getY());
	std::vector<coordinate> coordList;
	std::vector<coordinate> finalPath;
	coordinate enemyStart = {findYBlock(enemy.getY()), findXBlock(enemy.getX()), 0};
	coordList.push_back(enemyStart);
	bool keepGoing = false;
	bool inList = false;
	int count = 0;
	int coordListLength;

  if(abs(yPlayer - enemyStart.row) < 1 && abs(xPlayer - enemyStart.col) < 1){
    return coordList;
  }

	while(!keepGoing) {
    printf("generating path %d\n", count);
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
    if(count > 25){
      exit(0);
    }
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

int Enemy::findXBlock(float pos) {
  int center = pos + TANK_WIDTH/2;
	int trueX = center - TILE_SIZE - BORDER_GAP;
	int block = trueX / TILE_SIZE;
	return block;
}

int Enemy::findYBlock(float pos) {
  int center = pos + TANK_HEIGHT/2;
	int trueY = center - TILE_SIZE;
	int block = trueY / TILE_SIZE;
	return block;
}
