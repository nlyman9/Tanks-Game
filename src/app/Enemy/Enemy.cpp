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
  SDL_Rect pos = {x_enemy_pos, y_enemy_pos, TANK_WIDTH, TANK_HEIGHT};
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

  float x_pos = gPlayer->getX();
  float y_pos = gPlayer->getY();
  bool retreat;
  retreat = checkPos(x_pos, y_pos, x_enemy_pos, y_enemy_pos);

  bool nearWall;
  nearWall = checkWall(x_enemy_pos, y_enemy_pos);

  int startingPosition =  SCREEN_WIDTH - TANK_WIDTH/2 - 75;

  if(left) {
    if(tile_map[xArrPosL(x_enemy_pos)][yArrPos(y_enemy_pos)] == 2) {
      x_enemy_pos -= MAX_VELOCITY;
    } else {
      y_enemy_pos -= MAX_VELOCITY;
    }
  } else {
    if(tile_map[xArrPosR(x_enemy_pos)][yArrPos(y_enemy_pos)] == 2) {
      x_enemy_pos += MAX_VELOCITY;
    } else {
      y_enemy_pos -= MAX_VELOCITY;
    }
  }

  if(x_enemy_pos <= TILE_SIZE + BORDER_GAP) {
    left = false;
  }
  if(x_enemy_pos >= startingPosition) {
    left = true;
  }

  SDL_Rect* box = get_box(); // required to update box    //Enemy box is within 8 pixels of an obstacle
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