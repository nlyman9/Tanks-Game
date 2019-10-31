#include <iostream>
#include <vector>
#include "Tileset.hpp"
#include "QuadConstants.hpp"

Tileset::Tileset(vector_2d t, int n) {
    tiles = t;
    id = n;
}

Tileset::Tileset() {
    vector_2d v(0, vector_1d(0,0));
    Tileset(v, 0);
}

Tileset::~Tileset() {
    assert(!tiles.empty());
    delete &tiles;
}

vector_2d Tileset::get_tiles() {
    return tiles;
}

int Tileset::get_id() {
    return id;
}

void Tileset::print_tiles() {
    for (auto& t : tiles)
        for (auto& num : t)
            std::cout << num << " ";
        std::cout << std::endl;
}

// int main() {
//     return 0;
// }