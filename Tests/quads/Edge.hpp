#ifndef EDGE_HPP
#define EDGE_HPP

#include <vector>
#include "Tileset.hpp"
#include "QuadConstants.hpp"

class Edge {
  private:
    Tileset src, dest;
    int weight;
  public:
    Edge(Tileset, Tileset);
    Edge(Tileset, Tileset, int);
    ~Edge();
    int get_weight();
    void set_weight(int);
    Tileset get_src();
    Tileset get_dest();
    void print_edge();
};

#endif