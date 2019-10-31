#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <chrono>
#include <vector>
#include <cmath>
#include <SDL2/SDL.h>
#include "Object.hpp"

class Projectile : public OBJECT {
    private:
        /* data */
        Sprite missile;
		float x_vel = 0;
		float y_vel = 0;
		int bounces = 0;
		int theta = 0;
        int theta_v = 0;
        float x_deltav = 0;
        float y_deltav = 0;
        float velocity = 0;
        float delta_velocity = 0;
		bool friendly = false;

    public:
        Projectile(Sprite *missile, float x, float y); //constructor, initialize the x, y, and sprite
        Projectile(float x, float y); //constructor, initialize the x, y
		Projectile(float x, float y, int theta);	//constructor, initialize with x, y, and theta

		//Object methods
        void draw(SDL_Renderer *gRenderer, double update_lag) override;
        void update() override;
        bool move(float x, float y) override; //move x offset from current x and y offset from current y
        bool place(float x, float y) override; //place/teleport to an x and y

		int getTheta();
		bool rotateProjectile(float theta);	// rotates the projectile
		bool bouncePriority(int x, int y);
        bool wall();	//	return true if projectile hits a wall. X and Y projectile should change depending on orientation

        BoundingBox* getBoundingBox() override;
        
        ~Projectile();
};
#endif
