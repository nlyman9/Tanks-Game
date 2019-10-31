#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include "Tileset.hpp"
#include "Edge.hpp"
#include "Quadrant.hpp"

class Graph {
  private:
    std::vector<std::vector<Edge>> adj;
  public:
    Graph(std::vector<Edge>, int);
    Graph(int);
    Graph();
    ~Graph();
    void print_graph();
};

#endif