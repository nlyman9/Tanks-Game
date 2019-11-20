#ifndef QUADS_HPP
#define QUADS_HPP

#include <vector>
#include "Tileset.hpp"
#include "Edge.hpp"
#include "Quadrant.hpp"
#include "QuadConstants.hpp"

class Quads
{
  private:
    std::vector<Quadrant> quads;
    std::vector<Edge> edges;
    vector_2d flip(vector_2d);
    vector_2d mirror(vector_2d);
    vector_3d corner(vector_2d);
  public:
    Quads();
    void make_quads();
    std::vector<Quadrant> get_quads();
    std::vector<Edge> get_edges();
    void print_quads();
    void print_edges();
    int get_num_tilesets();
};

#endif