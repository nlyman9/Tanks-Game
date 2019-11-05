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

struct coordinate {
	int row;
	int col;
	int weight;
};

class Enemy : public OBJECT {
    private:
        /* data */
        // Sprite sprite;
        // Sprite turret;
				int randCut = 2;
        float x_enemy_pos, y_enemy_pos;
        Player* gPlayer;
        bool left = true;
        std::vector<coordinate> enemyPath;

        bool checkPos(float playX, float playY, float enemX, float enemY);
        bool checkWall(float x, float y);
				bool isValidBlock(int x, int y);
				coordinate findClosestOpenBlock(coordinate start);
		//	just wondering if we want to put some kind of custom acceleration on enemies?
		//	this is just a note for the future since the online pitt library won't let me open the book I found
		//	but maybe we'll eventually have a setting for the ai's current state (searching, pathing, running away, etc)
		    int xArrPosL(float pos);
        int xArrPosR(float pos);
        int yArrPos(float pos);
        int findXBlock(float pos);
        int findYBlock(float pos);
				coordinate newGhostPos(int gX, int gY, int eX, int eY);
        std::vector<coordinate> generatePath(std::vector<std::vector<int>> move_map, Player player, Enemy enemy);
        bool validMove(coordinate moveTo, coordinate currentlyAt);
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

        void setPathway(std::vector<std::vector<int>> move_map, Player player, Enemy enemy);
        void setTileMap(std::vector<std::vector<int>>* tileMap);
        ~Enemy();
};
#endif
