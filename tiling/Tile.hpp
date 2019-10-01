#ifndef TILE_HPP
#define TILE_HPP

#include <stdio.h>
#include <SDL2/SDL.h>

class Tile : public OBJECT {

    const int TILE_WIDTH = 48;
    const int TILE_HEIGHT = 48;
    
    public:
        Tile(int x, int y) {};
        ~Tile();
        void update() override;
        void draw() override;pu
        bool move(int x, int y) override;
        bool placec (int x, int y) override;

    private:
        int x, y;

};

OBJECT::~Tile() {
    std::cout << "Tile destructure" << std::endl;
}

#endif