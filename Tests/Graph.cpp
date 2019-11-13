#include <iostream>
#include <list>
using namespace std;

class Graph {
private:
  vector< vector<int> > graph;
public:
  Graph(int size) {
    graph.assign(size);
  }

  void addEdge(int src, int dest) {
    //add edges both ways, undirected graph
    graph[src].push_back(dest);
    graph[dest].push_back(src);
  }

  void removeEdge(int src, int dest) {
    int i = 0;
    for (int i = 0; i < graph[src].size(); i++) {
      if(graph[src][i] == dest) {
        //if the value is equal to the value we are deleting, erase from vector
        graph.erase(graph.begin() + i);
        break;
      }
    }
    //need to remove both edges, both sides because undirected
    for (int i = 0; i < graph[dest].size(); i++) {
      if(graph[dest][i] == src) {
        //if the value is equal to the value we are deleting, erase from vector
        graph.erase(graph.begin() + i);
        break;
      }
    }
  }

  bool hasEdge(int src, int dest) {
    int i = 0;
    for (auto x : graph[src]) {
      if(graph[src][i] == dest) {
        return;
      }
    }
    return false;
  }

  void BFS(int s) {
    int prev = -1;
    vector<bool> visited(graph.size);
    list<int> queue;
    list<int>::iterator i;
    vector< vector<int> >(graph.size()) BFS_graph;

    //none have been visited
    for(bool node: visited) {
      node = false;
    }
    //add source to visited
    visited[s] = true;
    queue.push_back(s);

    while(!queue.empty()) {
      s = queue.front();
      if(prev != -1) {
        //add edge to graph if not first iteration
        BFS_graph[s].push_back(prev);
        BFS_graph[prev].push_back(s);
      }
      queue.pop_front();

      for(i = graph[s].begin(); i != graph[s].end(); i++) {
        if(!visited[*i]) {
          visited[*i] = true;
          queue.push_back(*i);
        }
      }
      prev = s;
    }
    graph = BFS_graph;
  }
  //void addNode(int value) {}
}
