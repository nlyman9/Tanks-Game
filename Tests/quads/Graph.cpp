#include <vector>
#include <iostream>
#include "Graph.hpp"

Graph::Graph(std::vector<Edge> edges, int n) {
    adj.resize(n);
    for (auto& edge : edges)
        adj[edge.get_src_id()].push_back(edge);
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

int main() {
    Quads q;
    q.make_quads();
    // q.print_quads();
    // q.print_edges();
    Graph g(q.get_edges(), q.get_num_tilesets());
    g.print_graph();
    return 0;
}