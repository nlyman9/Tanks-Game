#ifndef GAMELOOP_HPP
#define GAMELOOP_HPP

#include <iostream>
#include <vector>
#include <chrono>
#include "Player.hpp"
#include "Render.hpp"
#include "Enemy.hpp"
#include "MapGenerator.hpp"
#include "Client.hpp"

class Args {
	public:
		bool isOnline;
		bool isHost;
		std::string ip;
		int port;
};

class GameLoop {
  public:
    GameLoop() {};
    ~GameLoop();
    bool networkInit(Args *options);
    bool init(Render* renderer);
    int runSinglePlayer();
    int networkRun();
    void initMapSinglePlayer();
    void initMapMultiPlayer();

  private:
    // Render class to render the game
    Render *render;
    Player *player;
    std::vector<Enemy *> enemies;
    bool isGameOn;
    Client* client;
    // Time delta of (current_time) - (previous_time)
    // The ratio sets the duration to calculate in Miliseconds
    std::chrono::duration<double, std::ratio<1, 1000>> elapsed_time;

    // Time since last update
    // May be a sumation of elapsed times on fast computers.
    double lag_time;
    
    // Previous time step of the gameloop
    std::chrono::system_clock::time_point previous_time;

    // Current time step of the gameloop
    std::chrono::system_clock::time_point current_time;

    // Map Building Variables
    std::vector<SDL_Rect> tileArray;

};
#endif