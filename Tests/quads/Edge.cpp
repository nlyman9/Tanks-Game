#include <iostream>
#include <vector>
#include "Edge.hpp"
#include "Tileset.hpp"
#include "QuadConstants.hpp"

Edge::Edge(Tileset s, Tileset d, int w) {
    src = s;
    dest = d;
    weight = w;
}

Edge::Edge(Tileset s, Tileset d) {
    Edge(s, d, 0);
}

Edge::~Edge() {
    delete &src;
    delete &dest;
}

int Edge::get_weight() {
    return weight;
}

void Edge::set_weight(int w) {
    weight = w;
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

void Edge::print_edge() {
    std::cout << src.get_id() << " -> " << dest.get_id() 
        << " weight: " << weight << std::endl;
}

// int main() {
//     return 0;
// }