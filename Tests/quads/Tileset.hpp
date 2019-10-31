#ifndef QUAD_HPP
#define QUAD_HPP

#include <vector>
#include <cassert>
#include "QuadConstants.hpp"

class Tileset {
  private:
    vector_2d tiles;
    int id;
  public:
    Tileset(vector_2d, int);
    Tileset();
    ~Tileset();
    vector_2d get_tiles();
    int get_id();
    void print_tiles();
};
#endif