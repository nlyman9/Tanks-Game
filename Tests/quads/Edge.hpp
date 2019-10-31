#ifndef EDGE_HPP
#define EDGE_HPP

#include <vector>
#include "Tileset.hpp"
#include "QuadConstants.hpp"

class Edge {
  private:
    Tileset src, dest;
    int multiplier;
    int weight;
  public:
    Edge(Tileset, Tileset, int, int);
    Edge(Tileset, Tileset, int);
    Edge(Tileset, Tileset);
    ~Edge();
    Tileset get_src();
    Tileset get_dest();
    int get_src_id();
    int get_dest_id();
    int get_multiplier();
    void set_multiplier(int);
    int get_weight();
    void set_weight(int);
    void print_edge();
};

#endif