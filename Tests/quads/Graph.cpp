#include <vector>
#include "Graph.hpp"

Graph::Graph(std::vector<Edge> edges, int n) {
    adj.resize(n);
    for (auto& edge : edges)
        adj[edge.get_src_id()].push_back(edge);
}

Graph::Graph(int n) {
    adj.resize(n);
}

Graph::~Graph() {
    delete[] &adj;
}

void Graph::print_graph() {
    for (auto& edges : adj)
        for (auto& edge : edges)
            edge.print_edge();
}

int main() {

}