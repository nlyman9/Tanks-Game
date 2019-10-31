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