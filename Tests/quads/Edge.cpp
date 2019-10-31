#include <iostream>
#include <vector>
#include "Edge.hpp"
#include "Tileset.hpp"
#include "QuadConstants.hpp"

Edge::Edge(Tileset s, Tileset d, int m, int w) {
    src = s;
    dest = d;
    multiplier = m;
    weight = w;
}

Edge::Edge(Tileset s, Tileset d, int m) {
    Edge(s, d, m, 0);
}

Edge::Edge(Tileset s, Tileset d) {
    Edge(s, d, 1);
}

Edge::~Edge() {
    delete &src;
    delete &dest;
}

Tileset Edge::get_src() {
    return src;
}

Tileset Edge::get_dest() {
    return dest;
}

int Edge::get_src_id() {
    return src.get_id();
}

int Edge::get_dest_id() {
    return dest.get_id();
}

int Edge::get_multiplier() {
    return multiplier;
}

void Edge::set_multiplier(int m) {
    multiplier = m;
}

int Edge::get_weight() {
    return weight;
}

void Edge::set_weight(int w) {
    weight = w;
}

void Edge::print_edge() {
    std::cout << src.get_id() << " -> " << dest.get_id() 
        << " weight: " << weight << std::endl;
}

// int main() {
//     return 0;
// }