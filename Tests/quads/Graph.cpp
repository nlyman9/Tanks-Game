#include <vector>
#include <iostream>
#include "Graph.hpp"

Graph::Graph(std::vector<Edge> e, std::vector<Quadrant> q, int n) {
    adj.resize(n);
    for (auto& edge : e)
        add_edge(edge);        // adj[edge.get_src_id()].push_back(edge);
    for (auto& quad : q)
        quads.push_back(quad);
}

Graph::Graph(int n) {
    adj.resize(n);
}

Graph::Graph() {
    adj.resize(10);
}

// Graph::~Graph() {
//     delete[] &adj;
// }

void Graph::add_edge(Edge e) {
    adj[e.get_src_id()].push_back(e);
}

bool Graph::edge_exists(Tileset s, Tileset d) {
    for (int i = 0; i < adj.size(); i++) {
        if (adj[i].size() > 0) {
            for (int j = 0; j < adj[i].size(); j++) {
                if (adj[i][j].get_src_id() == s.get_id() && adj[i][j].get_dest_id() == d.get_id()) return true;
            }
        }
    }
    return false;
}

/**
 * For each quadrant in quads:
 *   For each tileset in quadrant:
 *     Find if that tileset has edges for each tileset in the next quadrant     
 *        if an edge doesn't exist, create one
 **/ 

void Graph::populate_edges() {
    for (int i = 0; i < quads.size() - 2; i++) {
        for (int j = 0; j < quads[i].size(); j++) {
            if (!edge_exists(quads[i].get_tileset(j), quads[i + 1].get_tileset(j))) {
                Edge e(quads[i].get_tileset(j), quads[i + 1].get_tileset(j), 1, 0);
                adj[e.get_src_id()].push_back(e);
            }
        }
    }
}

/**
 *  Find the number of tilesets in each quadrant
 *  For each tileset in quadrant:
 *      Find it's respective edges
 *      Find the number of tilesets in the next quadrant it isn't connected to
 **/

// void Graph::calculate_weights() {
//     for (auto& quadrant : quads) {  
//         int tilesets = quadrant.size();
//     }
// }

void Graph::print_graph() {
    for (int i = 0; i < adj.size(); i++) {
        if (adj[i].size() > 0) {
            std::cout << "adj[" << i << "] ->";
            for (int j = 0; j < adj[i].size(); j++) {
                std::cout << " " << adj[i][j].get_dest_id();
            }
            std::cout << std::endl;
        }
    }
}

void Graph::print_quads_in_graph() {
    for (auto& q: quads) {
        q.print_quadrant();
    }
}

int main() {
    Quads q;
    q.make_quads();
    // q.print_quads();
    // q.print_edges();
    Graph g(q.get_edges(), q.get_quads(), q.get_num_tilesets());
    // g.populate_edges();
    g.print_graph();
    // g.calculate_weights();
    // g.print_quads_in_graph();
    return 0;
}