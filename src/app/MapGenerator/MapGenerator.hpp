#ifndef MAPGENERATOR_HPP
#define MAPGENERATOR_HPP

#include <vector>

constexpr int X_WIDE = 24;
constexpr int Y_HIGH = 13;

class MapGenerator{
  public:
    std::vector<std::vector<int>>* generateMap();
  private:
    std::vector<std::vector<int>> tile_map;
    std::vector<std::vector<int>> generateEmptyMap();
    std::vector<std::vector<int>> generateMirrorMap();
    std::vector<std::vector<int>> generateLineMap();
    std::vector<std::vector<int>> generateHMazeMap();
    int room[X_WIDE][Y_HIGH];
};


#endif