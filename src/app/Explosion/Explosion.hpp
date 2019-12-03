#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include <chrono>
#include <vector>
#include <cmath>
#include <SDL2/SDL.h>
#include "Object.hpp"

class Explosion : public Object {
	private:
		int frame = 0;
		int theta = 0;
		int anim_length = 0;
		Uint32 anim_last_time = 0;
		bool finished = false;
	
	public:
		Explosion(float x, float y); //constructor, initialize the x, y
		Explosion(int x, int y, int theta); //constructor, initialize the x, y, and theta angle for the sprite
		~Explosion(); //destructor
		
		//object methods
		void draw(SDL_Renderer *gRenderer, double update_lag) override;
        void update() override;
        bool move(float x, float y) override; //move x offset from current x and y offset from current y
        bool place(float x, float y) override; //place/teleport to an x and y
		BoundingBox* getBoundingBox() override;
		
		bool isFinished(); //returns the bool variable finished
};
#endif