#ifndef LOCALGAMELOOP_HPP
#define LOCALGAMELOOP_HPP

#include "Render.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Projectile.hpp"
#include <vector>

class LocalGameLoop {
    public:
        LocalGameLoop(Render* renderer);
        bool init();
        int run();

    private:
        // Member Variables 
        Player* player;
        std::vector<Enemy *> enemies;
    	std::vector<Projectile *> projectiles;

        Sprite* pinksplosion = nullptr;
        Sprite* enemy_tank = nullptr;
        Sprite* enemy_turret = nullptr;
        Sprite* bullet = nullptr;
        Sprite* shell = nullptr;
        SDL_Texture* cursor = nullptr;

        Render* render = nullptr;
        bool isGameOn = false;

        // Map Building Variables
        std::vector<SDL_Rect> tileArray;
        // Enemy needs to see holes as full block, otherwise it will run through them
        std::vector<SDL_Rect> enemyTileArray;
        // Projectiles don't see holes as obstacles, keep separate array
        std::vector<SDL_Rect> projectileObstacles;

        // Run variables
        std::chrono::duration<double, std::ratio<1, 1000>> elapsed_time;
        std::chrono::system_clock::time_point previous_time;
        std::chrono::system_clock::time_point current_time;
        double lag_time;
        unsigned int timer, begin_timer, current_timer;

        // Member Functions
        void generateMap();
        std::vector<int> spawnEnemies(std::vector<std::vector<int>> *map, int count);
        std::vector<int> spawnEnemy(std::vector<std::vector<int>> *map);

};


#endif 