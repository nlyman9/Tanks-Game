#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include "Tileset.hpp"
#include "Edge.hpp"
#include "Quadrant.hpp"
#include "Quads.hpp"
#include "QuadConstants.hpp"

class Graph {
  private:
    std::vector<std::vector<Edge>> adj;
    std::vector<Quadrant> quads;
  public:
    Graph(std::vector<Edge>, std::vector<Quadrant>, int);
    Graph(int);
    Graph();
    // ~Graph();
    void add_edge(Edge);
    bool edge_exists(Tileset, Tileset);
    void populate_edges();
    void calculate_weights();
    void print_graph();
    void print_quads_in_graph();
};

#endif