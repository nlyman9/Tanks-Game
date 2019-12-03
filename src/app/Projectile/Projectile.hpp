#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <chrono>
#include <vector>
#include <cmath>
#include <SDL2/SDL.h>
#include "Object.hpp"

class Projectile : public Object {
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
        int speedFactor = 1;
		bool friendly = false;
        bool exploding = false;
		int frame = 0;
		Uint32 anim_last_time = 0;
		bool finished = false;
		std::vector<SDL_Rect> targets;
		int targetNum = 0;
		bool hit = false;
		SDL_Rect targetBox;
        int id;
        
        std::vector<std::vector<int>> tile_array;
        int colTileX = -1;
        int colTileY = -1;

    public:
    
        int ticks = 0;
        
        Projectile(Sprite *missile, float x, float y); //constructor, initialize the x, y, and sprite
        Projectile(float x, float y); //constructor, initialize the x, y
		Projectile(float x, float y, int theta, int speed);	//constructor, initialize with x, y, and theta

		//Object methods
        void draw(SDL_Renderer *gRenderer, double update_lag) override;
        void update() override;
        bool move(float x, float y) override; //move x offset from current x and y offset from current y
        bool place(float x, float y) override; //place/teleport to an x and y

		int getTheta();
		bool rotateProjectile(float theta);	// rotates the projectile
		bool bouncePriority(SDL_Rect* A, SDL_Rect *B); // checks the side of the tile that the projectile is colliding with
        bool wall();	//	return true if projectile hits a wall. X and Y projectile should change depending on orientation

		bool isHit();
		bool isExploding();
		bool isFinished();
        void setFinished(bool fini);

		bool getFriendly(); // returns friendly
		bool setFriendly(bool a); // modifies and returns the value of friendly

        void setID(int i) {
            id = i;
        }
        int getID() {
            return id;
        }

        void addTargetLocation(SDL_Rect* targetLoc); // adds a target to the vector of targets
		void clearTargets(); // resets the vector list of targets
		SDL_Rect* getTarget();
        bool projCollisionCheck(Projectile* bullet2);
        void setExploding(bool explode);
        BoundingBox* getBoundingBox() override;

        // methods for breaking destructible blocks
        std::vector<int> getTilePosition(int pixelX, int pixelY);
        void setTileArray(std::vector<std::vector<int>> tile_array);
        bool hasDestructCollision();
        int getColX();
        int getColY();

        ~Projectile();
};
#endif
