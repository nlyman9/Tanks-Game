#include <chrono>
#include "Projectile.hpp"
//#include "Enemy.hpp"
#include "Constants.hpp"


Projectile::Projectile(Sprite *sprite, float x, float y) {
    setSprite(sprite);
    setPos(x, y);
}

Projectile::Projectile(float x, float y) {
    setPos(x, y);
}

Projectile::Projectile(float x, float y, int theta) {
	setPos(x, y);
	this->theta = theta;

	x_vel = 180 * cos((theta * M_PI) / 180);
	y_vel = 180 * sin((theta * M_PI) / 180);
}


Projectile::~Projectile() {
	/*
	delete &missile;
	delete &x_vel;
	delete &y_vel;
	delete &bounces;
	delete &theta;
    delete &theta_v;
    delete &x_deltav;
    delete &y_deltav;
    delete &velocity;
    delete &delta_velocity;
	delete &friendly;
	*/
}

void Projectile::draw(SDL_Renderer *gRenderer, double update_lag) {
    int x_pos = getX() + x_vel * update_lag;
    int y_pos = getY() + y_vel * update_lag;

    SDL_Rect* dst = get_box();
	dst->w = PROJECTILE_WIDTH;
	dst->h = PROJECTILE_HEIGHT;
    SDL_RenderCopyEx(gRenderer, getSprite()->getTexture(), NULL, dst, theta, NULL, SDL_FLIP_NONE);
}

void Projectile::update() {

    //rotateProjectile(theta_v);

	x_vel = 180 * cos((theta * M_PI) / 180);
	y_vel = 180 * sin((theta * M_PI) / 180);

    float updateStep = MS_PER_UPDATE/1000;
    setPos(getX() + (x_vel * updateStep), getY() + (y_vel * updateStep));

    SDL_Rect* overlap;
    SDL_Rect currentPos = {getX(), getY(), PROJECTILE_WIDTH + 1.41 * cos((theta * M_PI_4)/180), PROJECTILE_HEIGHT + 1.41 * cos((theta * M_PI_4)/180)};


    for(auto obstacle : obstacles) {

        overlap = check_collision(&currentPos, &obstacle);
        if(overlap != nullptr) {

			if (bounces == 3) {
				//should delete here.

			}

			theta_v = theta % 90;

			int x_dist = currentPos.x - obstacle.x;
			int y_dist = currentPos.y - obstacle.y;
			bool x_bounce = bouncePriority(x_dist, y_dist);

			//std::cout << "new\n";
			//std::cout << "x_dist = " << x_dist << " ; y_dist = " << y_dist << std::endl;

			if(x_bounce) { // collision left or right
				//std::cout << "SIDES" << std::endl;
				double num = -1 * cos((theta * M_PI) / 180);
				if(theta > 180)
					theta = 270 - (theta - 270);
				else
					theta = acos(num) * 180 / M_PI;
				//std::cout << "num = " << num << std::endl;
				//std::cout << "theta = " << theta << std::endl << std::endl;
			}
			else { // collision up or down
				//std::cout << "TOPSIES" << std::endl;
				double num = -1 * sin((theta * M_PI) / 180);
				if(theta > 180)
					theta = 270 - (theta - 270);
				else
					theta = asin(num) * 180 / M_PI;
				//std::cout << "num = " << num << std::endl;
				//std::cout << "theta = " << theta << std::endl << std::endl;
			}

			//std::cout << "x = " << currentPos.x << " ; y = " << currentPos.y << std::endl;
			//std::cout << "ob.x = " << obstacle.x << " ; ob.y = " << obstacle.y << std::endl;
			//std::cout << "ob.x + t.size = " << obstacle.x + TILE_SIZE << " ; ob.y + t.size = " << obstacle.y + TILE_SIZE << std::endl << std::endl;

			setPos(getX() - (x_vel * updateStep), getY() - (y_vel * updateStep));
			bounces++;

            break;
        }
    }

	if (getX() + PROJECTILE_WIDTH > SCREEN_WIDTH - TILE_SIZE - BORDER_GAP)	// Right border
    {
        setX(SCREEN_WIDTH - TILE_SIZE - PROJECTILE_WIDTH - BORDER_GAP);
		//std::cout << "SIDES" << std::endl;
		double num = -1 * cos((theta * M_PI) / 180);
		if(theta > 180)
			theta = 270 - (theta - 270);
		else
			theta = acos(num) * 180 / M_PI;

    }
    if (getX() < TILE_SIZE + BORDER_GAP)	// left border
    {
        setX(TILE_SIZE + BORDER_GAP);
		//std::cout << "SIDES" << std::endl;
		double num = -1 * cos((theta * M_PI) / 180);
		if(theta > 180)
			theta = 270 - (theta - 270);
		else
			theta = acos(num) * 180 / M_PI;

    }
    if (getY() < TILE_SIZE)	// Top border
    {
        setY(TILE_SIZE);
		double num = -1 * sin((theta * M_PI) / 180);
		if(theta > 180)
			theta = 270 - (theta - 270);
		else
			theta = asin(num) * 180 / M_PI;

    }
    if (getY() + PROJECTILE_HEIGHT > SCREEN_HEIGHT - TILE_SIZE)	// bottom border
    {
        setY(SCREEN_HEIGHT - TILE_SIZE - PROJECTILE_HEIGHT);
		double num = -1 * sin((theta * M_PI) / 180);
		if(theta > 180)
			theta = 270 - (theta - 270);
		else
			theta = asin(num) * 180 / M_PI;

    }
}

bool Projectile::bouncePriority(int x, int y) {
	int a, b;
	if(x < 0)
		return true;
	else if (y < 0)
		return false;

	if (x >= 24)
		a = 48 - x;
	if (y >= 24)
		b = 48 - y;

	if (a >= b)
		return true;
	else
		return false;
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

BoundingBox* Projectile::getBoundingBox() {
	return new BoundingBox();
}
