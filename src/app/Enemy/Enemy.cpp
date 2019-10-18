#include "Constants.hpp"
#include "Enemy.hpp"

// Enemy::Enemy(Sprite sprite, Sprite turret, int x, int y) : sprite{sprite}, turret{turret}, x_enemy_pos{x}, y_enemy_pos{y} {}

Enemy::Enemy(int x, int y, Player* player) :  x_enemy_pos{x}, y_enemy_pos{y}, gPlayer{player} {}

Enemy::~Enemy() {}


/**
 * @brief draws the Enemy object 
 *  Overrides base class OBJECT
 * 
 * @param update_lag - the value to extrapolate by
 */
void Enemy::draw(SDL_Renderer *gRenderer, double update_lag, std::vector<SDL_Rect> obstacles) {

  // Extrapolate the x and y positions 
  // "Solves" stuck in the middle rendering.
  // TODO change MAX_VELOCITY to the enemy's velocity
  
  // int x_pos = getX() + x_velocity * update_lag;
  // int y_pos = getY() + y_velocity * update_lag;

  // Render enemy
  SDL_Rect src = {0, 0, 48, 48};
  SDL_Rect dst = {x_enemy_pos, y_enemy_pos, 39, 48};
  SDL_RenderCopyEx(gRenderer, getSprite()->getTexture(), &src, &dst, 0, NULL, SDL_FLIP_NONE);
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
bool Enemy::move(int x, int y) {
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
bool Enemy::place(int x, int y) {
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

bool Enemy::checkPos(int playX, int playY, int enemX, int enemY) {
  double stepOne = (double)(pow((playX - enemX), 2) + pow((playY - enemY), 2));

	double distanceAway = (pow(stepOne, .5));

	if (distanceAway < 200.0)
	{
		return true;
	}
	return false;
}

bool Enemy::checkWall(int x, int y) {
  //left wall
	if (x <= 20)
	{
		return true;
	}
	//right wall
	else if (x >= SCREEN_WIDTH - 2 * BOX_WIDTH)
	{
		return true;
	}
	//top wall
	else if (y <= 20)
	{
		return true;
	}
	//bottom wall
	else if (y >= SCREEN_HEIGHT - 2 * BOX_HEIGHT)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Enemy::getX(){
  return x_enemy_pos;
}

int Enemy::getY(){
  return y_enemy_pos;
}

void Enemy::updatePos() {

  int x_pos = gPlayer->getX();
  int y_pos = gPlayer->getY();
  bool retreat;
  retreat = checkPos(x_pos, y_pos, x_enemy_pos, y_enemy_pos);

  bool nearWall;
  nearWall = checkWall(x_enemy_pos, y_enemy_pos);

  if (!retreat)
  {

    //go to center

    if (x_enemy_pos <= SCREEN_WIDTH / 2)
    {
      x_enemy_pos += MAX_VELOCITY;
    }
    else
    {
      x_enemy_pos += -MAX_VELOCITY;
    }
    if (y_enemy_pos <= SCREEN_HEIGHT / 2)
    {
      y_enemy_pos += MAX_VELOCITY;
    }
    else
    {
      y_enemy_pos += -MAX_VELOCITY;
    }
  }
  else
  {

    //run away but not near wall
    if (!nearWall)
    {
      if (x_pos >= x_enemy_pos)
      {
        x_enemy_pos += -MAX_VELOCITY;
      }
      else
      {
        x_enemy_pos += MAX_VELOCITY;
      }
      if (y_pos >= y_enemy_pos)
      {
        y_enemy_pos += -MAX_VELOCITY;
      }
      else
      {
        y_enemy_pos += MAX_VELOCITY;
      }
    }
    else
    {
      //run away and on wall

      if (x_enemy_pos == 20 || x_enemy_pos == SCREEN_WIDTH - 2 * BOX_WIDTH)
      {
        if (y_pos >= y_enemy_pos)
        {
          y_enemy_pos += -MAX_VELOCITY;
        }
        else
        {
          y_enemy_pos += MAX_VELOCITY;
        }
      }
      if (y_enemy_pos == 20 || y_enemy_pos == SCREEN_HEIGHT - 2 * BOX_HEIGHT)
      {

        if (x_pos >= x_enemy_pos)
        {
          x_enemy_pos += -MAX_VELOCITY;
        }
        else
        {
          x_enemy_pos += MAX_VELOCITY;
        }
      }
    }
  }
  if (x_enemy_pos > SCREEN_WIDTH - 2 * BOX_WIDTH)
  {
    x_enemy_pos = SCREEN_WIDTH - 2 * BOX_WIDTH;
  }
  if (x_enemy_pos < 20)
  {
    x_enemy_pos = 20;
  }
  if (y_enemy_pos < 20)
  {
    y_enemy_pos = 20;
  }
  if (y_enemy_pos > SCREEN_HEIGHT - 2 * BOX_HEIGHT)
  {
    y_enemy_pos = SCREEN_HEIGHT - 2 * BOX_HEIGHT;
  }
}