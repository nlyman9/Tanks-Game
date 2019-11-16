#include "Constants.hpp"
#include "Enemy.hpp"
#include <math.h>

/**
  * @brief full constructor for enemY
  * all parameters should be self explanatory
  */
Enemy::Enemy(Sprite* sprite, Sprite* turret, int x, int y, Player* player){
  setSprite(sprite);
  setTurretSprite(turret);
  setPos(x, y);
  gPlayer = player;
}

/**
  * @brief smaller constructor for making an enemY
  */
Enemy::Enemy(float x, float y, Player* player) : x_enemy_pos{x}, y_enemy_pos{y} {
	setPos(x, y);
	gPlayer = player;
}

Enemy::~Enemy() {}


/**
 * @brief draws the Enemy object
 *  Overrides base class Object
 *
 * @param update_lag - the value to extrapolate by
 */
 void Enemy::draw(SDL_Renderer *gRenderer, double update_lag) {

   // Create SDL_Rect with current x, y position
   SDL_Rect dst = {(int)x_enemy_pos, (int)y_enemy_pos, TANK_WIDTH, TANK_HEIGHT};

   if (/*x_vel != 0 || y_vel != 0 && */SDL_GetTicks() - anim_last_time > 100) {
		frame = (frame + 1) % 3;
		anim_last_time = SDL_GetTicks();
	}

  //draw the tank and turret and use the rect from above as parameters for both calls since turret is centered on tank
   SDL_RenderCopyEx(gRenderer, getSprite()->getTexture(), getSprite()->getFrame(frame), &dst, theta, NULL, SDL_FLIP_NONE);
   SDL_RenderCopyEx(gRenderer, getTurretSprite()->getTexture(), NULL, &dst, turretTheta, NULL, SDL_FLIP_NONE);

   //lines coming out of turret "filed of view" have end points and this method finds them
   findEndValues(turretTheta);

   //draw the two lines with endpoints that were just calculated
   SDL_RenderDrawLine(gRenderer, x_enemy_pos + TANK_WIDTH/2, y_enemy_pos + TANK_HEIGHT/2, line1X, line1Y);
   SDL_RenderDrawLine(gRenderer, x_enemy_pos + TANK_WIDTH/2, y_enemy_pos + TANK_HEIGHT/2, line2X, line2Y);
 }


/**
  * @brief takes current turretTheta and finds endpoints of lines of filed of view
  *
  * @param angle : current turretTheta
  */
 void Enemy::findEndValues(float angle){
   //multiply turretTheta by -1 to get degrees measured in standard way with counter-clockwise rotation around circle
   angle *= -1;
   //convert to radians
   angle = angle * M_PI / 180;
   //radius of unit circle used for calculations
   //set as SCREEN_WIDTH so range always bigger than screen
   int length = SCREEN_WIDTH;
   //adjust given angle by +- .25 to create a range
   float ang1 = angle + .25;
   float ang2 = angle - .25;
   //caclulate endpoints using unit circle trig
   //sets global variables of each line's endpoints so no return vals
   line1X = cos(ang1) * length + getX();
   line1Y = getY() - sin(ang1) * length;
   line2X = cos(ang2) * length + getX();
   line2Y = getY() - sin(ang2) * length;
 }

/**
 * @brief update the Enemy object
 *  Overrides base class Object
 *
 */
void Enemy::update() {
  updatePos();
}

/**
 * @brief move the Enemy an offset from its current x-y position
 *  Overrides base class Object
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

/**
  * @brief calculates the area of a triangle
  * @params the x, y coordinates of the three corners
  * @return the area
  */
float Enemy::area(float x1, float y1, float x2, float y2, float x3, float y3){
  return abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0);
}

/**
  * @brief checks if a given point is within a triangle
  * @params the x, y coordinates of the triangle corners and the x,y of the point checking
  * @return true if point is within triangle, false otherwise
  */
bool Enemy::isInRange(float x1, float y1, float x2, float y2, float x3, float y3, float playerX, float playerY){
  //overall area
  float A = area(x1, y1, x2, y2, x3, y3);
  //trianlge PBC
  float a1 = area(playerX, playerY, x2, y2, x3, y3);
  //triangle PAC
  float a2 = area(x1, y1, playerX, playerY, x3, y3);
  //trianlge PAB
  float a3 = area(x1, y1, x2, y2, playerX, playerY);

  //sum the three smaller areas
  float sum = a1 + a2 + a3;

  //if the sum is within 1 of overall area then point is inside big triangle
  //otherwise it is not and return false
  if(abs(A - sum) < 1){
    return true;
  }
  else{
    return false;
  }
}

/**
  * @brief used to check if distance between two x,y points is less than a given number
  * @params x,y positions of two points to check against
  * @return true if distance between is smaller than given number, false otherwise
  */
bool Enemy::checkPos(float playX, float playY, float enemX, float enemY) {
  //calculate all of distance formula except square root
  double stepOne = (double)(pow((playX - enemX), 2) + pow((playY - enemY), 2));
  //do the square root step
	double distanceAway = (pow(stepOne, .5));

  //check if distance is < 200 and if so return true
	if (distanceAway < 200.0)
	{
		return true;
	}
	return false;
}

float Enemy::getX(){
  return x_enemy_pos;
}

float Enemy::getY(){
  return y_enemy_pos;
}


/**
  * @brief used to update the position of the enemy tank including x,y position and turretTheta
  * additionally checks for outer wall bounds collisions and corrects for those
  * utilizes shortest path mehtod (generatePath) to develop its latest path
*/
void Enemy::updatePos() {
  //set the current time as this will be used to check whether or not to carry out certain actions based on elapsed time
  Uint32 current_time = SDL_GetTicks();

  //checking if player tank is "in range" of enemy tanks field of view
  if(isInRange(x_enemy_pos + TANK_WIDTH/2, y_enemy_pos + TANK_HEIGHT/2, line1X, line1Y, line2X, line2Y, gPlayer->getX() + TANK_WIDTH/2, gPlayer->getY() + TANK_HEIGHT/2)){
    //only allowed to shoot every 3 seconds so current time must be greater than last fired time
    if(current_time > fire_last_time + 3000){
      setFire(true);
      //reset fire_last_time to current time so that can fire again in the future
      fire_last_time = current_time;
    }
  }

  //change turret mode every 3 seconds
  //trackOrMonitor will be true if track mode and false if monitor mode
  if(current_time > turret_mode_change + 3000){
    //randonly pick between track and monitor modes
    //track mode will always point the turret towards the player tank
    //monitor mode will spin the enemy tank's turret
    if(rand() % 2 == 0){
      trackOrMonitor = true;
    }
    else{
      trackOrMonitor = false;
    }
    //reset turret_mode_change so can change again in the future
    turret_mode_change = current_time;
  }
  //track mode
  if(trackOrMonitor){
    //hi frems
    //Use trig to calculate the correct angle the turret should point at based on player location
    float curTheta = getTurretTheta();
    float delta_x = (gPlayer->getX() + TANK_WIDTH / 2) - (getX() + TANK_WIDTH / 2);
    float delta_y = (gPlayer->getY() + TANK_HEIGHT / 2) - (getY() + TANK_HEIGHT / 2);
    float theta_radians = atan2(delta_y, delta_x);
    turretTheta = (int)(theta_radians * 180 / M_PI);
  }
  //monitor mode
  else{
    //since turretTheta is -180 to 180 degrees need to account for that by using global rotateUp bool
    //rotateUp will be true if spinning 0 to 180 degrees
    //rotateUp will be false if spinning -180 to 0 degrees
    //add 1 to turretTheta each updatePos call to have a good speed
    if(rotateUp){
      turretTheta += 1;
      //when turretTheta eclipses 180, set it to -180 (same actual pos so still spins seemlessly) and switch rotateUp bool
      if(turretTheta > 180){
        turretTheta = -180;
        rotateUp = false;
      }
    }
    else{
      turretTheta += 1;
      //similar to above, since in negatives, when gets > 0 switch back to rotateUp being true
      if(turretTheta > 0){
        turretTheta = 0;
        rotateUp = true;
      }
    }
  }

  //change states of the enemy tank every 5 seconds
  //utilize the global wander boolean
  //if wander is true then tank will randomly wander around map
  //if wander is false then tank will "follow" or move towards the player
  if(current_time > last_state_change + 5000){
    //give it a one in three chance of entering wander mode
    if(rand() % 3 == 0){
      wander = true;
    }
    else{
      wander = false;
    }
    //need to clear the current path it is on so that it can create a new one based on state change
    enemyPath.clear();
    //update last_state_change time so that can update in the future
    last_state_change = current_time;
  }

  //the enemy path it is following will need to be updated frequently when the size of the vector is below a value
  //this value is global randCut variable that will randonly change what this cutoff is in the range 2-6
  //this is done so to give the enemy some sense of randomness as it will change how far it travels down its current path before updating
  //range low bound is 2 because need at least two points in pathway to have tank move
  //once the path size is less than randCut then set a new pathway for the enemy to follow and reassign randCut
  if(enemyPath.size() < randCut){
    setPathway(this->tile_map, *this->gPlayer, *this);
    randCut = rand() % 4 + 2;
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

  //Get direction of current direction moving
  if (theta == 0) {
    moveRight = true;
    rightLeft = true;
  }
  else if (theta == 90){
    moveDown = true;
    upDown = true;
  }
  else if (theta == 180) {
    moveLeft = true;
    rightLeft = true;
  }
  else if (theta == 270) {
    moveUp = true;
    upDown = true;
  }


  //main moving function of the tank based on the path it currently has
  //need check that size is > 1 since utilize pop and can't pop if size is < 1
  if(enemyPath.size() > 1){

    //these two coordinates will be how the enemy picks which direction to move
    //because of pathing algorithm these coordinates will always be adjacent on the map
    coordinate moveFrom = enemyPath[enemyPath.size() - 1];
    coordinate moveTo = enemyPath[enemyPath.size() - 2];

    //move LEFT
    if(moveFrom.col > moveTo.col){
      if (moveLeft || rightLeft) {
        x_enemy_pos -= MAX_VELOCITY;
        y_enemy_pos += 0;
        //check that the tank position arrived in desired location
        if(x_enemy_pos < (moveTo.col * TILE_SIZE + TILE_SIZE + 36)){
          enemyPath.pop_back();
        }
        //check collision with player
        if (overlap != nullptr) {
          x_enemy_pos += MAX_VELOCITY;
        }
      }
      else {
        //don't move and rotate the tank by updating the theta
        if (theta > 180) {
          theta -= PHI;
        }
        else {
          theta += PHI;
        }
        setFalse();
      }
    }
    //move RIGHT
    if(moveFrom.col < moveTo.col){
      if (moveRight || rightLeft) {
        x_enemy_pos += MAX_VELOCITY;
        y_enemy_pos += 0;
        //check that the tank position arrived in desired location
        if(x_enemy_pos > (moveTo.col * TILE_SIZE + TILE_SIZE*2 - 20)){
          enemyPath.pop_back();
        }
        //check collision
        if (overlap != nullptr) {
          x_enemy_pos -= MAX_VELOCITY;
        }
      }
      else {
        if (theta > 270) {
          //If pointed up, rotate right
          theta += PHI;
          if (theta == 360) { //ensure it can go to the right now
            theta = 0;
          }
        }
        else {
          //If pointed down, rotate left
          theta -= PHI;
        }
        setFalse();
      }
    }
    //move UP
    if(moveFrom.row > moveTo.row){
      if (moveUp || upDown) {
        x_enemy_pos += 0;
        y_enemy_pos -= MAX_VELOCITY;
        //check that the tank position arrived in desired location
        if(y_enemy_pos < (moveTo.row * TILE_SIZE + TILE_SIZE + 20)){
          enemyPath.pop_back();
        }
        //check collision
        if (overlap != nullptr) {
          y_enemy_pos += MAX_VELOCITY;
        }
      }
      else {
        if (theta > 270) {
          theta -= PHI;
        }
        else {    //theta has to be 0 here and since 0 - 3 would not work, set to 360
          theta = 360;
          theta -= PHI;
        }
        setFalse();
      }
    }
    //move DOWN
    if(moveFrom.row < moveTo.row){
      if (moveDown || upDown) {
        x_enemy_pos += 0;
        y_enemy_pos += MAX_VELOCITY;
        //check that the tank position arrived in desired location
        if(y_enemy_pos > (moveTo.row * TILE_SIZE + TILE_SIZE + 20)){
          enemyPath.pop_back();
        }
        //check collision
        if (overlap != nullptr) {
          y_enemy_pos -= MAX_VELOCITY;
        }
      }
      else {
        if (theta > 90) {
          theta -= PHI;
        }
        else {
          theta += PHI;
        }
        setFalse();
      }
    }
  }
  //run checks to see if enemy tank went outside bounds of play area
  if (x_enemy_pos + TANK_WIDTH > SCREEN_WIDTH - TILE_SIZE - 16)
  {
      x_enemy_pos = SCREEN_WIDTH - TILE_SIZE - 16 - TANK_WIDTH;
  }
  if (x_enemy_pos < TILE_SIZE + 16)
  {
      x_enemy_pos = TILE_SIZE + 16;
  }
  if (y_enemy_pos < TILE_SIZE)
  {
      y_enemy_pos = TILE_SIZE;
  }
  if (y_enemy_pos + TANK_HEIGHT > SCREEN_HEIGHT - TILE_SIZE)
  {
      y_enemy_pos = SCREEN_HEIGHT - TILE_SIZE - TANK_HEIGHT;
  }
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

/**
  * @brief develop a pathway for the enmy tank to follow
  * @params the game map, the player, and the enemy tank
  * do to needing the shortest path to run quickly a "ghost" tank needs to be created as the target location for the enemy
  */
void Enemy::setPathway(std::vector<std::vector<int>> move_map, Player player, Enemy enemy){
  //The ghost X and Y block are first initialized to the players location
  //the findXBlock and findYBlock methods will give tile block location that the tank is in
  //will be 0-23 for findXBlock and 0-12 for findYBlock
  int ghostXblock = findXBlock(player.getX());
  int ghostYblock = findYBlock(player.getY());
  //Do the same thing but for the current enemy position
  int enemyXblock = findXBlock(enemy.getX());
  int enemyYblock = findYBlock(enemy.getY());
  //if the player and enemy are within 2 blocks in the X and Y AND not in wander mode then just use the actual player tank to generate the path
  //will not need to calculate a ghost tank since close enough algorithm will run quickly
  if(abs(ghostXblock - enemyXblock) < 2 && abs(ghostYblock - enemyYblock) < 2 && !wander){
    enemyPath = generatePath(move_map, player, enemy);
  }
  //otherwise will need to generate the path using a ghost tank
  else{
    //whether or not in wander or follow mode need to make a ghost location
    //main difference is if ghost location is towards the player (!wander) or in a random direction (wander)
    if(wander){
      coordinate randGhostPos = Enemy::randGhostPos(enemyXblock, enemyYblock);
      //adjust return values from x,y tile block locations to actual x,y positions on the play area
      int randGhostX = randGhostPos.col * TILE_SIZE + TILE_SIZE + BORDER_GAP;
      int randGhostY = randGhostPos.row * TILE_SIZE + TILE_SIZE;
      //make the ghost tank given the new x, y positions
      Player* randGhost = new Player(randGhostX, randGhostY, true);
      //generate the path using this ghost tank
      enemyPath = generatePath(move_map, *randGhost, enemy);
    }
    else{
      coordinate newGhostPos = Enemy::newGhostPos(ghostXblock, ghostYblock, enemyXblock, enemyYblock);
      //adjust return values from x,y tile block locations to actual x,y positions on the play area
      int ghostX = newGhostPos.col * TILE_SIZE + TILE_SIZE + BORDER_GAP;
      int ghostY = newGhostPos.row * TILE_SIZE + TILE_SIZE;
      //make the ghost tank given the new x, y positions
      Player* ghost = new Player(ghostX, ghostY, true);
      //generate the path using this ghost tank
      enemyPath = generatePath(move_map, *ghost, enemy);
    }
  }
}

/**
  * @brief used to randomly generate a direction for the ghost tank to spawn
  * @params the TILE block locations of the enemy tank
  * @return the coordinate of the ghost tank
  */
coordinate Enemy::randGhostPos(int eX, int eY){
  //randomly pick a direction in the x and y for the ghost to go
  int randX = rand() % 3;
  int randY = rand() % 3;
  //start coordinate of the ghost tank = the enemy tank
  //will be adjusted later but needs to start at enemy tank so path is short
  coordinate newPos = {eY, eX, 0};

  //if randX == 0 or 1 then +- 1 from newPos.row
  //if == 2 then ghost will be in the same row
  if(randX == 0){
    newPos.row -= 1;
  }
  else if(randX == 1){
    newPos.row += 1;
  }

  //if randY == 0 or 1 then +- from newPos.col
  //if == 2 then ghost will be in the same col
  if(randY == 0){
    newPos.col -= 1;
  }
  else if(randY == 1){
    newPos.col += 1;
  }

  //need end path location to be a valid moveable space on the map
  //if it is not adjust the ghost location using findClosestOpenBlock and then return newPos
  if(!isValidBlock(newPos.row, newPos.col)){
    newPos = findClosestOpenBlock(newPos);
  }
  return newPos;

}

/**
  * @brief used to find ghost location that is in direction towards the player tank
  * @params the TILE block locations of the enemy and player tanks
  * @return the coordinate of the ghost tank
  */
coordinate Enemy::newGhostPos(int gX, int gY, int eX, int eY){
  //start newPos coordinate where the enemy currently is
  coordinate newPos = {eY, eX, 0};
  //if the tanks are in the same block, return newPos since enemy tank shouldn't move b/c can't get closer
  if(gX == eX && gY == eY){
    return newPos;
  }
  //adjust row and col of newPos based on relationship between inputs
  //player above enemy
  if(gY < eY){
    //player left of enemy
    if(gX < eX){
      //move up and to the left
      newPos.row -= 1;
      newPos.col -= 1;
    }
    //player right of enemy
    else if(gX > eX){
      //move up and to the right
      newPos.row -= 1;
      newPos.col += 1;
    }
    //player above only
    else{
      newPos.row -= 1;
    }
  }
  //player below enemy
  else if(gY > eY){
    if(gX < eX){
      //move down and to the left
      newPos.row += 1;
      newPos.col -= 1;
    }
    else if(gX > eX){
      //move down and to the right
      newPos.row += 1;
      newPos.col += 1;
    }
    else{
      newPos.row += 1;
    }
  }
  //player horizontally equal with enemy
  else{
    if(gX < eX){
      newPos.col -= 1;
    }
    else{
      newPos.col += 1;
    }
  }
  //need end path location to be a valid moveable space on the map
  //if it is not adjust the ghost location using findClosestOpenBlock and then return newPos
  if(!isValidBlock(newPos.row, newPos.col)){
    newPos = findClosestOpenBlock(newPos);
  }
  return newPos;
}

/**
  * @brief used to find the closet block to a given location that is moveable to
  * @param start : the block location in question
  * @return the coordinate of the block that is closest and moveable to
  */
coordinate Enemy::findClosestOpenBlock(coordinate start){
  coordinate test = {start.row, start.col, 0};
  //count increments once every while loop call to cycle between the options for adjacency
  int count = 0;
  //increment is the block distance away currently checking on
  int increment = 1;
  //check until find a block that isValid
  //test by incrementing/decrementing test.row/test.col and then resetting those values back when check fails
  while(1){
    //check above
    if(count%8 == 0){
      test.row -= increment;
      if(isValidBlock(test.row, test.col)){
        return test;
      }
      test.row += increment;
    }
    //check above & right
    else if(count%8 == 1){
      test.row -= increment;
      test.col += increment;
      if(isValidBlock(test.row, test.col)){
        return test;
      }
      test.row += increment;
      test.col -= increment;
    }
    //check right
    else if(count%8 == 2){
      test.col += increment;
      if(isValidBlock(test.row, test.col)){
        return test;
      }
      test.col -= increment;
    }
    //check below & right
    else if(count%8 == 3){
      test.row += increment;
      test.col += increment;
      if(isValidBlock(test.row, test.col)){
        return test;
      }
      test.row -= increment;
      test.col -= increment;
    }
    //check below
    else if(count%8 == 4){
      test.row += increment;
      if(isValidBlock(test.row, test.col)){
        return test;
      }
      test.row -= increment;
    }
    //check below & left
    else if(count%8 == 5){
      test.row += increment;
      test.col -= increment;
      if(isValidBlock(test.row, test.col)){
        return test;
      }
      test.row -= increment;
      test.col += increment;
    }
    //check left
    else if(count%8 == 6){
      test.col -= increment;
      if(isValidBlock(test.row, test.col)){
        return test;
      }
      test.col += increment;
    }
    //check above & left
    else {
      test.row -= increment;
      test.col -= increment;
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

/**
  * @brief used to test if a tile block location is valid (moveable to)
  * @params x, y are tile block locations in question
  * @return true if a valid block and falase if not valid
  */
bool Enemy::isValidBlock(int x, int y){
  //find curX and curY to be the block locations that the enemy tank is currently in
  int curX = findXBlock(x_enemy_pos);
  int curY = findYBlock(y_enemy_pos);
  //if location in question is outside the bounds of the map return false
  if(y < 0 || y > 23 || x < 0 || x > 12){
    return false;
  }
  //if tile_map location is a 0 means can access this location
  //second check determines block location in question does not equal the current location of the enemy tank
  //don't want to allow this to return true or else enemy won't move
  //if these two conditions are met then return true, otherwise false
  if((tile_map[y][x] == 0) && !(y == curX && x == curY)){
    return true;
  }
  return false;
}

/**
  * @brief helper method to generatePath that will be used in building the pathway
  * @params the coordinate to move towards and the coodinate the path is currentlyAt
  * @return true if a valid move and false otherwise
  */
bool Enemy::validMove(coordinate moveTo, coordinate currentlyAt){
  //find the row, col, and weight of the inputs
	int moveToRow = moveTo.row;
	int moveToCol = moveTo.col;
	int moveToWeight = moveTo.weight;
	int atRow = currentlyAt.row;
	int atCol = currentlyAt.col;
	int atWeight = currentlyAt.weight;

	//if weights valid - one less than weight currently at
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


/**
  * @brief generate the shortest path from the enemy to the player
  * @params the tile_map, the player to move to, and the enemy to move from
  * @return a vector list of coordinates from the enemy to the player
  */
std::vector<coordinate> Enemy::generatePath(std::vector<std::vector<int>> move_map, Player player, Enemy enemy){
  //caclulate the x and y tile block locations of the player that will be used as endpoints to traverse to
	int xPlayer = findXBlock(player.getX());
	int yPlayer = findYBlock(player.getY());
	std::vector<coordinate> coordList;
	std::vector<coordinate> finalPath;
  //make the first coordinate in the list the location of the enemy
	coordinate enemyStart = {findYBlock(enemy.getY()), findXBlock(enemy.getX()), 0};
	coordList.push_back(enemyStart);
  //initialize other values
	bool keepGoing = false;
	bool inList = false;
  //count used as check to see if proper weight value is seen
	int count = 0;
	int coordListLength;

  //if player is in same location as enemy then return coordList since enemy shouldn't move since already close as possible
  if(abs(yPlayer - enemyStart.row) < 1 && abs(xPlayer - enemyStart.col) < 1){
    return coordList;
  }

	while(!keepGoing) {
	  //add surrounding squares to list - left, right, up, down
    //only add if
      // 1. not already in the list
      // 2. not a border
      // 3. an open tile
    //if all of these are true add it to the coordList
		coordListLength = coordList.size();
		for(int i = 0; i < coordListLength; i++){
			if(coordList[i].weight == count){
				coordinate left = {coordList[i].row, coordList[i].col - 1, coordList[i].weight + 1};
				//check isnt a wall
				if(!(left.row < 0 || left.row > 12 || left.col < 0 || left.col > 23) && (move_map[left.col][left.row] == 0)){
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
				if(!(right.row < 0 || right.row > 12 || right.col < 0 || right.col > 23) && (move_map[right.col][right.row] == 0)){
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
				if(!(up.row < 0 || up.row > 12 || up.col < 0 || up.col > 23) && (move_map[up.col][up.row] == 0)){
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
				if(!(down.row < 0 || down.row > 12 || down.col < 0 || down.col > 23) && (move_map[down.col][down.row] == 0)){
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

  //finally add the coordinate of the player with weight = count
	coordinate currentCoord = {yPlayer, xPlayer, count};
	finalPath.push_back(currentCoord);
	coordListLength = coordList.size();

  //work backwards from player location to enemy location to develop shortest path
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

	return finalPath;
}

/**
  * @brief used to find tile block location in the X based on pos
  * @param pos : position to use
  * @return tile block that pos is in, range 0-23
  */
int Enemy::findXBlock(float pos) {
  int center = pos + TANK_WIDTH/2;
	int trueX = center - TILE_SIZE - BORDER_GAP;
	int block = trueX / TILE_SIZE;
	return block;
}


/**
  * @brief used to find tile block location in the Y based on pos
  * @param pos : position to use
  * @return tile block that pos is in, range 0-12
  */
int Enemy::findYBlock(float pos) {
  int center = pos + TANK_HEIGHT/2;
	int trueY = center - TILE_SIZE;
	int block = trueY / TILE_SIZE;
	return block;
}

void Enemy::setFalse() {
  moveUp = false;
  moveDown = false;
  moveLeft = false;
  moveRight = false;
  rightLeft = false;
  upDown = false;
}
