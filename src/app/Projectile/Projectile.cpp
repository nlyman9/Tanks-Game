#include <chrono>
#include "Projectile.hpp"
#include "Constants.hpp"

Projectile::Projectile(Sprite *sprite, float x, float y) {
    setSprite(sprite);
    setPos(x, y);
}

Projectile::Projectile(float x, float y) {
    setPos(x, y);
}

Projectile::Projectile(float x, float y, int theta, int speed) {
	setPos(x, y);
	this->theta = theta;
  this->speedFactor = speed;

	x_vel = 180 * cos((theta * M_PI) / 180);
	y_vel = 180 * sin((theta * M_PI) / 180);
}


Projectile::~Projectile() {
}

void Projectile::draw(SDL_Renderer *gRenderer, double update_lag) {
    int x_pos = getX() + x_vel * update_lag;
    int y_pos = getY() + y_vel * update_lag;
    SDL_Rect* dst = get_box();

	if(!exploding) {
		dst->w = PROJECTILE_WIDTH;
		dst->h = PROJECTILE_HEIGHT;
		SDL_RenderCopyEx(gRenderer, getSprite()->getTexture(), NULL, dst, theta, NULL, SDL_FLIP_NONE);
	}
	else {
		finished = true;
	}
}

bool Projectile::isHit(){
	return this->hit;
}

bool Projectile::isExploding(){
	return this->exploding;
}

bool Projectile::isFinished() {
	return this->finished;
}

void Projectile::update() {
	if(theta < 0)
		theta = theta + 360;

	int collisionTheta = theta;

    //rotateProjectile(theta_v);

	targetNum = 0;

	if(!exploding) {
		x_vel = speedFactor * 180 * cos((theta * M_PI) / 180);
		y_vel = speedFactor * 180 * sin((theta * M_PI) / 180);

		float updateStep = MS_PER_UPDATE/1000;
		setPos(getX() + (x_vel * updateStep), getY() + (y_vel * updateStep));

		SDL_Rect* overlap;
		SDL_Rect currentPos = {(int)getX(),(int) getY(), PROJECTILE_WIDTH, PROJECTILE_HEIGHT};

		for(auto target : targets) {
			overlap = check_collision(&currentPos, &target);
			if (overlap != nullptr) {
				hit = true;
				targetBox = target;
				exploding = true;
				break;
			}
			targetNum++;
		}

		for(auto obstacle : obstacles) {
			overlap = check_collision(&currentPos, &obstacle);
			if(overlap != nullptr) {

				// destructible terrain code
				std::vector<int> coordinate = getTilePosition(obstacle.x, obstacle.y);
				this->colTileX = coordinate.at(0);
				this->colTileY = coordinate.at(1);

				theta_v = theta % 90;

				int x_dist = currentPos.x - obstacle.x;
				int y_dist = currentPos.y - obstacle.y;
				bool x_bounce = bouncePriority(&currentPos, &obstacle);

				if(x_bounce) { // collision left or right
					double num = -1 * cos((theta * M_PI) / 180);
					if(theta > 180)
						theta = 270 - (theta - 270);
					else
						theta = acos(num) * 180 / M_PI;
				}
				else { // collision up or down
					double num = -1 * sin((theta * M_PI) / 180);
					if(theta > 90 && theta < 270 || theta < -90)
						theta = 180 - asin(num) * 180 / M_PI;
					else
						theta = asin(num) * 180 / M_PI;
				}
				setPos(getX() - (x_vel * updateStep), getY() - (y_vel * updateStep));
				bounces++;

				break;
			}
		}

		if (getX() + PROJECTILE_WIDTH > SCREEN_WIDTH - TILE_SIZE - BORDER_GAP)	// Right border
		{
			setX(SCREEN_WIDTH - TILE_SIZE - PROJECTILE_WIDTH - BORDER_GAP);
			double num = -1 * cos((theta * M_PI) / 180);
					if(theta > 180)
						theta = 270 - (theta - 270);
					else
						theta = acos(num) * 180 / M_PI;
			bounces++;
		}
		if (getX() < TILE_SIZE + BORDER_GAP)	// left border
		{
			setX(TILE_SIZE + BORDER_GAP);
			double num = -1 * cos((theta * M_PI) / 180);
					if(theta > 180)
						theta = 270 - (theta - 270);
					else
						theta = acos(num) * 180 / M_PI;
			bounces++;
		}
		if (getY() < TILE_SIZE)	// Top border
		{
			setY(TILE_SIZE);
			double num = -1 * sin((theta * M_PI) / 180);
			if(theta > 90 && theta < 270)
				theta = 180 - asin(num) * 180 / M_PI;
			else
				theta = asin(num) * 180 / M_PI;
			bounces++;
		}
		if (getY() + PROJECTILE_HEIGHT > SCREEN_HEIGHT - TILE_SIZE)	// bottom border
		{
			setY(SCREEN_HEIGHT - TILE_SIZE - PROJECTILE_HEIGHT);
			double num = -1 * sin((theta * M_PI) / 180);
			if(theta > 90 && theta < 270)
				theta = 180 - asin(num) * 180 / M_PI;
			else
				theta = asin(num) * 180 / M_PI;
			bounces++;
		}
		//check if the bullet is still alive comment out for bounce testing
		if (bounces == 4) {
			exploding = true;
			theta = collisionTheta;
		}
	}
	else {
		x_vel = 0;
		y_vel = 0;

		float updateStep = MS_PER_UPDATE/1000;
		setPos(getX() + (x_vel * updateStep), getY() + (y_vel * updateStep));
	}
}

bool Projectile::bouncePriority(SDL_Rect* proj, SDL_Rect* obst) {
	int leftFacing = obst->x + TILE_SIZE - proj->x;
	int rightFacing = proj->x + PROJECTILE_WIDTH - obst->x;
	int botFacing = proj->y + PROJECTILE_HEIGHT - obst->y;
	int topFacing = obst->y + TILE_SIZE - proj->y;//abs(proj->y - (obst->y + TILE_SIZE));

	int cnt = 0;

	while(cnt <= 48) {
		if(leftFacing == cnt || rightFacing == cnt)
			return true;
		else if(botFacing == cnt || topFacing == cnt)
			return false;
		cnt++;
	}

	return true;
}

bool Projectile::move(float x, float y) {
    return false;
}

bool Projectile::place(float x, float y) {
    return false;
}

bool Projectile::rotateProjectile(float theta) {
    this->theta += theta;
    return true;
}

int Projectile::getTheta() {
	return theta;
}

bool Projectile::getFriendly() {
	return friendly;
}

bool Projectile::setFriendly(bool a) {
	friendly = a;
	return friendly;
}

void Projectile::clearTargets() {
	targets.clear();
}

BoundingBox* Projectile::getBoundingBox() {
	return new BoundingBox();
}

void Projectile::addTargetLocation(SDL_Rect* targetLoc) {
	targets.push_back(*targetLoc);
}

SDL_Rect* Projectile::getTarget() {
    return &targetBox;
}

void Projectile::setExploding(bool explode){
  this->exploding = explode;
}

void Projectile::setBounces(int numBounces){
  this->bounces = numBounces;
}

bool Projectile::projCollisionCheck(Projectile* bullet2){
  SDL_Rect projBox1 = {(int)getX(), (int)getY(), PROJECTILE_WIDTH, PROJECTILE_HEIGHT};
  SDL_Rect projBox2 = {(int)bullet2->getX(), (int)bullet2->getY(), PROJECTILE_WIDTH, PROJECTILE_HEIGHT};
  return (check_collision(&projBox1, &projBox2));
}

void Projectile::setFinished(bool fini) {
  this->finished = fini;
}

void Projectile::setTileArray(std::vector<std::vector<int>> array)
{
	tile_array = array;
}

// Given the pixel x,y coordinates of an obstacle, returns the tile coordinates of the obstacle.
// Returns: std::vector<int> length two, first value is X, second is Y coordinate
std::vector<int> Projectile::getTilePosition(int pixelX, int pixelY)
{
	int tileX = (pixelX - 64) / 48;
	int tileY = (pixelY - 48) / 48;

	std::vector<int> coordinates;

	coordinates.push_back(tileX);
	coordinates.push_back(tileY);

	return coordinates;
}

bool Projectile::hasDestructCollision()
{
	if(colTileX >= 0 &&
	   colTileY >= 0)
	{
		if(tile_array[colTileX][colTileY] >= 3)
		{
			return true;
		}
	}

	return false;
}

int Projectile::getColX()
{
	return colTileX;
}

int Projectile::getColY()
{
	return colTileY;
}
