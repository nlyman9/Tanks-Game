#ifndef QUADRANT_HPP
#define QUADRANT_HPP

#include <vector>
#include "Tileset.hpp"
#include "QuadConstants.hpp"

class Quadrant {
  private:
    std::vector<Tileset> quadrant;
    int id;
  public:
    Quadrant(int);
    // ~Quadrant();
    std::vector<Tileset> get_quadrant();
    bool tileset_exists(int);
    bool tileset_exists(Tileset);
    Tileset get_tileset(int);
    Tileset get_tileset(Tileset);
    int size();
    int get_id();
    void add(Tileset);
    void print_quadrant();
};
#endif