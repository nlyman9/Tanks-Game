#ifndef ONLINEGAMELOOP_HPP
#define ONLINEGAMELOOP_HPP

#include "Render.hpp"
#include "Args.hpp"
#include "Client.hpp"

class OnlineGameLoop {
    public:
        OnlineGameLoop(Render* renderer);
        bool init(Args* options);
        int run();
    private:
        // Member Variables 
        std::vector<Player *> players;
        std::vector<Player *> playerEnemies;
    	std::vector<Projectile *> projectiles;
        std::vector<Bomb *> bombs;

        Sprite* pinksplosion = nullptr;
		Sprite* redsplosion = nullptr;
		Sprite* bluesplosion = nullptr;
        Sprite* bullet = nullptr;
        Sprite* shell = nullptr;
        Sprite* bombBlack = nullptr;
        Sprite* bombRed = nullptr;
        Sprite* bombPlayerExplosion = nullptr;
        Sprite* bombEnemyExplosion = nullptr;
        SDL_Texture* cursor = nullptr;
        SDL_Texture* loadingScreen1 = nullptr;
        SDL_Texture* loadingScreen2 = nullptr;
        SDL_Texture* loadingScreen3 = nullptr;
        SDL_Texture* loadingScreen4 = nullptr;
        SDL_Texture* loadingScreen5 = nullptr;
        SDL_Texture* loadingScreen6 = nullptr;
        SDL_Texture* loadingScreen7 = nullptr;

        // Map Building Variables
        std::vector<SDL_Rect> tileArray;    
        // Projectiles don't see holes as obstacles, keep separate array
        std::vector<SDL_Rect> projectileObstacles;

        Render* render = nullptr;

        // Run variables
        std::chrono::duration<double, std::ratio<1, 1000>> elapsed_time;
        std::chrono::system_clock::time_point previous_time;
        std::chrono::system_clock::time_point current_time;
        double lag_time;

        // Network Variables
        int server_pid;
        Client* client;

        // Member Functions
        void buildMap();
        void displayLoadingScreen(int screenCounter);

};

#endif