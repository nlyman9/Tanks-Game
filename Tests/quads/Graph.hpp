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
    void calculate_weight(std::vector<Edge>, int);
  public:
    std::vector<std::vector<Edge>> adj;
    Graph(std::vector<Edge>, std::vector<Quadrant>, int);
    Graph(int);
    Graph();
    void add_edge(Edge);
    Edge get_edge(int, int);
    std::vector<std::vector<Edge>>* get_adj();
    std::vector<Edge>* get_from_adj(int);
    bool edge_exists(int, int);
    bool edge_exists(Tileset, Tileset);
    int select_edge_from_weight(std::vector<Edge>, int);
    void populate_edges();
    void calculate_weights();
    int select_from_quadrant(int, int);
    std::vector<Tileset> get_tiles_for_map();
    void print_graph();
    void print_quads_in_graph();
};

#endif