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
#include <vector>
#include "Object.hpp"
#include "Player.hpp"

class Enemy : public OBJECT {
    private:
        /* data */
        // Sprite sprite;
        // Sprite turret;
        int x_enemy_pos, y_enemy_pos;
        Player* gPlayer;

        bool checkPos(int playX, int playY, int enemX, int enemY);
        bool checkWall(int x, int y);
		//	just wondering if we want to put some kind of custom acceleration on enemies?
		//	this is just a note for the future since the online pitt library won't let me open the book I found
		//	but maybe we'll eventually have a setting for the ai's current state (searching, pathing, running away, etc)
		
    public:
        // Enemy(Sprite sprite, Sprite turret, int x, int y); //constructor, initialize the x, y, and sprite
        Enemy(int x, int y, Player* player); //constructor, initialize the x, y, and sprite

		//Object methods
        void draw(SDL_Renderer *gRenderer, double update_lag) override;
        void update() override;
        bool move(int x, int y) override;	//	move x offset from current x and y offset from current y
        bool place(int x, int y) override;	//	place/teleport to an x and y
        
        bool fire();	//	return true if the enemy fired successfully
        bool rotateEnemy(float theta);	//	rotate the object
        bool rotateTurret(float theta);	//	rotate the turret
        
        int getX();
        int getY();
        void updatePos();

        ~Enemy();
};
#endif