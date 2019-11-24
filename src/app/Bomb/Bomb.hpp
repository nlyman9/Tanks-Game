#ifndef BOMB_HPP
#define BOMB_HPP

#include "Object.hpp"
#include <chrono>

const auto COUNT_DOWN_NSEC = 3000000000;
const auto BOMB_DROP_PADDING = 5;

class Bomb : public Object {
    public:
        Bomb(SDL_Rect* box, int theta, Sprite* bombBlack, Sprite* bombRed, Sprite* explosion);
        ~Bomb();

        void draw(SDL_Renderer *gRenderer, double update_lag) override;
        void update() override;
        bool move(float x, float y) override {} // unused
        bool place(float x, float y) override {} // unused
        BoundingBox* getBoundingBox() override {} // unused

        bool getFinished() {return finished;}
        bool isExploding() {return exploding;}
    private:
        // Timer
        std::chrono::steady_clock::time_point startTime;
        std::chrono::steady_clock::time_point currTime;
        std::vector<long> flipIntervals;
        int flipCounter = 0;

        // Exploding Flags
        bool exploding = false;
        bool finished = false;

        // Animation Variables
        int frame = 0;
		Uint32 anim_last_time = 0;
        bool flipColor = true;

        Sprite* blackBomb = nullptr;
        Sprite* redBomb = nullptr;
        Sprite* bombExplosion = nullptr;
};

#endif