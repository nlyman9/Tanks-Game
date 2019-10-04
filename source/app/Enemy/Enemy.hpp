/**
 * @file enemy.hpp
 * @author Danny Phan
 * @brief Header for abstract enemy class
 * @version 0.1
 * @date 2019-09-30
 * 
 * @copyright Copyright (c) 2019
 * 
 */
 

#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Object.hpp"

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

class Enemy : public OBJECT {
    private:
        /* data */
        Enemy(SDL_Window* window, SDL_Renderer* renderer) : gWindow{window}, gRenderer{renderer} {};
        Sprite sprite;
        Sprite turret;
        Weapon weapon; 
        int x, y;
        //Enemy box
        x_enemy_pos = SCREEN_WIDTH / 2 - BOX_WIDTH / 2;
        y_enemy_pos = SCREEN_HEIGHT / 2 - BOX_HEIGHT / 2;
		//	just wondering if we want to put some kind of custom acceleration on enemies?
		//	this is just a note for the future since the online pitt library won't let me open the book I found
		//	but maybe we'll eventually have a setting for the ai's current state (searching, pathing, running away, etc)
		
    public:
        Enemy(Sprite sprite, Sprite turret, int x, int y); //constructor, initialize the x, y, and sprite
        Enemy(int x, int y); //constructor, initialize the x, y, and sprite

		//Object methods
        void draw(float update_lag) override;
        void update(int playerX, int playerY) override;
        bool move(int x, int y) override;	//	move x offset from current x and y offset from current y
        bool place(int x, int y) override;	//	place/teleport to an x and y
        
        bool fire();	//	return true if the enemy fired successfully
        bool rotateEnemy(float theta);	//	rotate the object
        bool rotateTurret(float theta);	//	rotate the turret

        ~Enemy();
};
#endif
