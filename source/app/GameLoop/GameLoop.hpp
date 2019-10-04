#ifndef GAMELOOP_HPP
#define GAMELOOP_HPP

#include <iostream>
#include <vector>
#include "Player.hpp"
#include "Render.hpp"

class GameLoop {
  public:
    //change to enemy object
    int y_enemy_pos;
    int x_enemy_pos;
    //change to other/object?
    int y_obst_pos;
    int x_obst_pos;
    GameLoop() {};
    ~GameLoop();
    int run();
};

#endif