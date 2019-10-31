#include <iostream>
#include <iterator>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <utility>
#include <vector>
#include "Quads.hpp"
#include "QuadConstants.hpp"
#include "Map.hpp"

void Map::print_map() {
    int count = 0;
    std::cout << "    ";
    for (int i = 0; i < MAP_WIDTH; i++) {
        std::cout << i % 10 << " ";
    }
    std::cout << std::endl << std::endl;
    for (auto& vector : map) {
        std::cout << (count++) % 10 << "   ";
        for (auto& num : vector) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
}

vector_3d Map::get_from(const vector_4d quads, int quadrant) {
    std::cout << "Getting Quadrant " << quadrant << std::endl;
    vector_3d q = quads.at(quadrant);
    for (auto quad : q)
        q.push_back(quad);
    return q;
}

void Map::assign_quad(vector_2d quad, int min_height, int max_height, int width_offset, int height_offset) {
    for (int i = min_height; i <= max_height; i++) {
        map[i].insert(map[i].begin()+width_offset, quad[i-height_offset].begin(), quad[i-height_offset].end());
        map[i].resize(MAP_WIDTH);
    }
}

void Map::make_map(const vector_4d quads) {

    vector_3d quadrant_1, quadrant_2, quadrant_3, 
                quadrant_4, quadrant_5, quadrant_6, quadrant_all;

    quadrant_1   = get_from(quads, 0);
    quadrant_2   = get_from(quads, 1);
    quadrant_3   = get_from(quads, 2);
    quadrant_4   = get_from(quads, 3);
    quadrant_5   = get_from(quads, 4);
    quadrant_6   = get_from(quads, 5);
    quadrant_all = get_from(quads, 6);
    
    srand(time(NULL));

    for (int i = 0; i < quads.size() - 1; i++) {
        int j = rand() % (quads[i].size() - 1);
        assign_quad(quads[i][j], MIN_HEIGHTS[i], MAX_HEIGHTS[i], WIDTH_OFFSETS[i], HEIGHT_OFFSETS[i]);
    }
}

Map::Map() {
    vector_2d temp(MAP_HEIGHT, vector_1d(MAP_WIDTH, 0));
    this->map = temp;
}

vector_2d Map::get_map() {
    return map;
}

int main() {
    Quads q = Quads();
    std::cout << "Made quads" << std::endl;
    vector_4d quads = q.get_quads();
    std::cout << "Got quads" << std::endl;
    Map m = Map();
    // m.make_map(quads);
    std::cout << "Made map" << std::endl;
    // m.print_map();
    return 0;
}
