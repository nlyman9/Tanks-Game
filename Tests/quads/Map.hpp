#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include "QuadConstants.hpp"

vector_2d get_map();

class Map {
  private:
    vector_2d map;
    vector_3d get_from(const vector_4d, int);
    void assign_quad(vector_2d, int, int, int, int);
  public:
    Map();
    void make_map(const vector_4d);
    vector_2d get_map();
    void print_map();
};

#endif