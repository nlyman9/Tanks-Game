//Projectile.http
//Ben Kurzyna
//Definitely lost writing this and am basing it off of the Enemy.hpp file (ty Danny)
//Feel free to update file wherever you see improvements

#include "Object.hpp"

#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

class Enemy : public OBJECT {
    private:
        /* data */
        Sprite missile;
        Weapon weapon;
        int x, y;
        //projectile velocity? are we having different projectiles?

    public:
        Projectile(Sprite missile, int x, int y); //constructor, initialize the x, y, and sprite
        Projectile(int x, int y); //constructor, initialize the x, y

		//Object methods
        void draw(float update_lag) override;
        void update() override;
        bool move(int x, int y) override;	//	move x offset from current x and y offset from current y
        bool place(int x, int y) override;	//	place/teleport to an x and y

        bool wall();	//	return true if projectile hits a wall. X and Y projectile direction should flip

        ~Projectile();
};
#endif
