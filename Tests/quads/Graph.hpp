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
    std::vector<Quadrant> quads;
    void calculate_weight(std::vector<Edge>);
  public:
    std::vector<std::vector<Edge>> adj;
    Graph(std::vector<Edge>, std::vector<Quadrant>, int);
    Graph(int);
    Graph();
    void add_edge(Edge);
    Edge get_edge(int, int);
    std::vector<std::vector<Edge>>* get_adj();
    bool edge_exists(int, int);
    bool edge_exists(Tileset, Tileset);
    void populate_edges();
    void calculate_weights();
    void print_graph();
    void print_quads_in_graph();
};

#endif