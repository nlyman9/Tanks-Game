#include <vector>
#include "Quadrant.hpp"

Quadrant::Quadrant(int n) {
    id = n;
}

// Quadrant::~Quadrant() {
//     assert(!quadrant.empty());
//     delete &quadrant;
// }

std::vector<Tileset> Quadrant::get_quadrant() {
    return quadrant;
}

bool Quadrant::tileset_exists(int id) {
    for (auto& tileset : quadrant) {
        if (tileset.get_id() == id) return true; 
    }
    return false;
}

bool Quadrant::tileset_exists(Tileset t) {
    return tileset_exists(t.get_id());
}

Tileset Quadrant::get_tileset(int id) {
    // if (!tileset_exists(id)) return nullptr;
    // for (auto& tileset : quadrant) {
    //     if (tileset.get_id() == id) {
    //         return tileset;
    //     }
    // }
    // return quadrant[0]; // return default
    return quadrant[id];
}

Tileset Quadrant::get_tileset(Tileset t) {
    return get_tileset(t.get_id());
}

int Quadrant::size() {
    return quadrant.size();
}

int Quadrant::get_id() {
    return id;
}

void Quadrant::add(Tileset t) {
    quadrant.push_back(t);
}

void Quadrant::print_quadrant() {
    for (auto& tileset : quadrant)
        tileset.print_tiles();
}

// int main() {
//     return 0;
// }