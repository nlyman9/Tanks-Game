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
#include "Graph.hpp"

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

void Map::make_map(std::vector<Tileset> tiles) {
    for (int i = 0; i <= tiles.size() - 1; i++) {
        assign_quad(tiles[i].get_tiles(), MIN_HEIGHTS[i], MAX_HEIGHTS[i], WIDTH_OFFSETS[i], HEIGHT_OFFSETS[i]);
    }
}

Map::Map() {
    vector_2d temp(MAP_HEIGHT, vector_1d(MAP_WIDTH, 0));
    this->map = temp;
}

vector_2d Map::get_map() {
    return map;
}
