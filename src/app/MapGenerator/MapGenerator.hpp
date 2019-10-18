#ifndef MAPGENERATOR_HPP
#define MAPGENERATOR_HPP

constexpr int X_WIDE = 24;
constexpr int Y_HIGH = 13;

class MapGenerator{
  public:
    int** generateLineMap();
    int** generateMirrorMap();
  private:
    int room[X_WIDE][Y_HIGH];
};


#endif