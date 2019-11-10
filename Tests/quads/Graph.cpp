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

Edge Graph::get_edge(int s_id, int d_id) {
    for (int i = 0; i < adj.size(); i++) {
        if (adj[i].size() > 0) {
            for (int j = 0; j < adj[i].size(); j++) {
                if (adj[i][j].get_src_id() == s_id && adj[i][j].get_dest_id() == d_id) {
                    return adj[i][j];
                }
            }
        }
    }
}

std::vector<std::vector<Edge>>* Graph::get_adj() {
    return &adj;
}

bool Graph::edge_exists(int s_id, int d_id) {
    for (int i = 0; i < adj.size(); i++) {
        if (adj[i].size() > 0) {
            for (int j = 0; j < adj[i].size(); j++) {
                if (adj[i][j].get_src_id() == s_id && adj[i][j].get_dest_id() == d_id) {
                    return true;
                }
            }
        }
    }
    return false;
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
    for (int i = 0; i < quads.size() - 1; i++) {
        for (int j = 0; j < quads[i].size(); j++) {
            for (int k = 0; k < quads[i + 1].size(); k++) {
                if (!edge_exists(quads[i].get_tileset(j).get_id(), quads[i + 1].get_tileset(k).get_id())) {
                    Edge e(quads[i].get_tileset(j), quads[i + 1].get_tileset(k), 1, 0);
                    // std::cout << "Adding edge between " << e.get_src_id() << " and " << e.get_dest_id() << std::endl;
                    adj[e.get_src_id()].push_back(e);
                }// } else {
                //     std::cout << "Edge already exists between " << quads[i].get_tileset(j).get_id()
                //         << " and " << quads[i + 1].get_tileset(k).get_id() << std::endl;
                // }
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

void Graph::calculate_weight(std::vector<Edge> edges) {
    int size = 0, div = 0, remainder = 0, mult = 0, mult_count = 0;
    size = edges.size();
    for (int i = 0; i < size; i++) {
        mult = edges[i].get_multiplier();
        mult_count += mult;
    }
    std::cout << "mult_count: " << mult_count << std::endl;
    if (mult_count != 0) {
        div = 100 / mult_count;
        remainder = 100 % mult_count;
        for (int i = 0; i < size; i++) {
            mult = edges[i].get_multiplier();
            if (remainder > 0) {
                edges[i].set_weight((mult * div) + remainder);
                remainder = 0;
            } else {
                edges[i].set_weight(mult * div);
            }
        std::cout << "Edge[" << i << "] w: " << edges[i].get_weight() << std::endl;
        }
    }
}

void Graph::calculate_weights() {
    for (auto& v : adj) {
        calculate_weight(v);
    }
}

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
    g.populate_edges();
    // std::vector<std::vector<Edge>> edge = g.get_adj();
    // g.calculate_weight(g.adj[0]);
    // g.
    // if (g.edge_exists(0, 1)) { 
    //     std::cout << "Edge exists" << std::endl;
    // } else {
    //     std::cout << "Edge does not exist" << std::endl;
    // }
    g.calculate_weights();
    g.print_graph();
    // g.calculate_weights();
    // g.print_quads_in_graph();
    return 0;
}