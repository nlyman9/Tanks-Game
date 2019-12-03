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
#include "Projectile.hpp"
#include "Bomb.hpp"
#include "Tank.hpp"

struct coordinate {
	int row;
	int col;
	int weight;
};

class Enemy : public Object, public Tank {
    private:
        /* data */
        Player* gPlayer;
        bool left = true;
		bool anim_last_time = 0;
        std::vector<coordinate> enemyPath;
		std::vector<Projectile *> enemyProjectiles;
		std::vector<Bomb *> bombList;
		std::vector<Enemy *> enemyList;

		float line1X, line1Y, line2X, line2Y;
		int randCut = 2;
		int updateCalls = 0;
		bool trackOrMonitor = false;
		bool rotateUp = true;
		bool moveUp = false;
		bool moveDown = false;
		bool moveLeft = false;
		bool moveRight = false;
		bool rightLeft = false;
		bool upDown = false;
		bool dodgingBullet = false;
		bool enemyOverlapCheck = false;
		int moveState = 0;
		int bulletXblock, bulletYblock, bulletTheta;
		int bombXblock, bombYblock;
		int enemyType;
		int shouldExplode = 0; //determine when purple tank explodes
		double velocity;

		Uint32 last_state_change = 0;
		Uint32 turret_mode_change = 0;

        bool checkPos(float playX, float playY, float enemX, float enemY, float range);
        bool checkWall(float x, float y);
		//	just wondering if we want to put some kind of custom acceleration on enemies?
		//	this is just a note for the future since the online pitt library won't let me open the book I found
		//	but maybe we'll eventually have a setting for the ai's current state (searching, pathing, running away, etc)
		void findEndValues(float angle);
		void setFalse();	//reset direction booleans to false
		int xArrPosL(float pos);
        int xArrPosR(float pos);
        int yArrPos(float pos);
        int findXBlock(float pos);
        int findYBlock(float pos);
		float area(float x1, float y1, float x2, float y2, float x3, float y3);
		bool isInRange(float x1, float y1, float x2, float y2, float x3, float y3, float playerX, float playerY);
		bool isValidBlock(int x, int y);
		coordinate findClosestOpenBlock(coordinate start);
		coordinate newGhostPos(int gX, int gY, int eX, int eY);
		coordinate randGhostPos(int eX, int eY);
		coordinate awayGhostPos(int eX, int eY, int bX, int bY, int bT);
		coordinate awayBombGhostPos(int eX, int eY, int bX, int bY);
		bool normalCheck(coordinate loc, std::vector<std::vector<int>> move_map);
		bool spiderCheck(coordinate loc, std::vector<std::vector<int>> move_map);
        std::vector<coordinate> generatePath(std::vector<std::vector<int>> move_map, Player player, Enemy enemy);
				bool validMove(coordinate moveTo, coordinate currentlyAt);
        std::vector<std::vector<int>> tile_map;

		//void playExplosion(SDL_Renderer *gRenderer, double update_lag);


    public:
		Enemy(Sprite* sprite, Sprite* turret, int x, int y, Player* player); //constructor, initialize the x, y, and sprite
        Enemy(float x, float y, Player* player, int type); //constructor, initialize the x, y, and sprite

		//Object methods
        void draw(SDL_Renderer *gRenderer, double update_lag) override;
        void update() override;
        bool move(float x, float y) override;	//	move x offset from current x and y offset from current y
        bool place(float x, float y) override;	//	place/teleport to an x and y
				int getEnemyType();
        void updatePos();
				void setProjectiles(std::vector<Projectile *> projectiles);	//set the projectiles vector from localgameloop.cpp
				void setBombs(std::vector<Bomb *> bombs); //set the bombs vector from LocalGameLoop.cpp
				void setEnemies(std::vector<Enemy *> enemies); //set the enemies vector from localgameloop.cpp

        BoundingBox* getBoundingBox() override;

        void setPathway(std::vector<std::vector<int>> move_map, Player player, Enemy enemy);
        void setTileMap(std::vector<std::vector<int>>* tileMap);
        ~Enemy();
};
#endif
