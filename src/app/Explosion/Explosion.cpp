#include <chrono>
#include "Explosion.hpp"
#include "Constants.hpp"
#include <math.h>

Explosion::Explosion(int x, int y, int theta) {
    setPos(x, y);
	this->theta = theta;
}

/*
Explosion::Explosion(float x, float y, int length) {
    setPos(x, y);
	anim_length = length;
}
*/

Explosion::~Explosion() {
}

void Explosion::draw(SDL_Renderer *gRenderer, double update_lag) {
	if(!finished) {
		SDL_Rect* dst = get_box();
		dst->w = EXPLOSION_WIDTH;
		dst->h = EXPLOSION_HEIGHT;	
		SDL_RenderCopyEx(gRenderer, getSprite()->getTexture(), getSprite()->getFrame(frame), dst, theta, NULL, SDL_FLIP_NONE);
	}
}

void Explosion::update() {
	if(frame > 6)
		finished = true;
	else {
		Uint32 current_time = SDL_GetTicks();
		//animating first frame
		if(frame == 0 && anim_last_time == 0) {
			anim_last_time = SDL_GetTicks();
		}

		//every next frame
		if(current_time > anim_last_time + 200) {
			//std::cout << frame << " ";
			frame++;
			anim_last_time = SDL_GetTicks();
		}
	}
}

bool Explosion::move(float x, float y) {
    return false;
}

bool Explosion::place(float x, float y) {
    return false;
}

BoundingBox* Explosion::getBoundingBox() {
	return new BoundingBox();
}

bool Explosion::isFinished() {
	return finished;	
}
