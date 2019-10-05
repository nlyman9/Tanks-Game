#ifndef GAMELOOP_HPP
#define GAMELOOP_HPP

#include <iostream>
#include <vector>
#include "Player.hpp"
#include "Render.hpp"
#include "Enemy.hpp"

class GameLoop {
  public:
    GameLoop() {};
    ~GameLoop();
    bool init();
    int run();

  private:
    // Render class to render the game
    Render *render;
    Player *player;
    std::vector<Enemy *> enemies;
    bool isGameOn;
};

#endif