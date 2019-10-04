#ifndef GAMELOOP_HPP
#define GAMELOOP_HPP

#include <iostream>
#include <vector>
#include "Player.hpp"
#include "Render.hpp"
#include "Enemy.hpp"

class GameLoop {
  public:
    // Render class to render the game
    Render *render;
    Player *player;
    std::vector<Enemy *> enemies;
    bool isGameOn;

    // //change to enemy object
    // int y_enemy_pos;
    // int x_enemy_pos;
    // //change to other/object?
    // int y_obst_pos;
    // int x_obst_pos;
    GameLoop() {};
    ~GameLoop();
    bool init();
    int run();
};

#endif