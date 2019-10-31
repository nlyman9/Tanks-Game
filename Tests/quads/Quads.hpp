#ifndef QUADS_HPP
#define QUADS_HPP

#include <vector>
#include "QuadConstants.hpp"

class Quads
{
  private:
    vector_4d quads;
    vector_2d flip(vector_2d);
    vector_2d mirror(vector_2d);
    vector_3d corner(vector_2d);
    void make_quads();

  public:
    Quads();
    vector_4d get_quads();
    void print_quads();
};

#endif