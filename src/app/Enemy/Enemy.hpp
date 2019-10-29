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
        float x_enemy_pos, y_enemy_pos;
        Player* gPlayer;
        bool left = true;

        bool checkPos(float playX, float playY, float enemX, float enemY);
        bool checkWall(float x, float y);
		//	just wondering if we want to put some kind of custom acceleration on enemies?
		//	this is just a note for the future since the online pitt library won't let me open the book I found
		//	but maybe we'll eventually have a setting for the ai's current state (searching, pathing, running away, etc)
		int xArrPosL(float pos);
        int xArrPosR(float pos);
        int yArrPos(float pos);
        std::vector<std::vector<int>> tile_map;
    public:
        // Enemy(Sprite sprite, Sprite turret, int x, int y); //constructor, initialize the x, y, and sprite
        Enemy(float x, float y, Player* player); //constructor, initialize the x, y, and sprite

		//Object methods
        void draw(SDL_Renderer *gRenderer, double update_lag) override;
        void update() override;
        bool move(float x, float y) override;	//	move x offset from current x and y offset from current y
        bool place(float x, float y) override;	//	place/teleport to an x and y
        
        bool fire();	//	return true if the enemy fired successfully
        bool rotateEnemy(float theta);	//	rotate the object
        bool rotateTurret(float theta);	//	rotate the turret
        
        float getX();
        float getY();
        void updatePos();

        BoundingBox* getBoundingBox() override;

        void setTileMap(std::vector<std::vector<int>>* tileMap);
        ~Enemy();
};
#endif