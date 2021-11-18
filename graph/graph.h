#pragma once

#include <memory>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Graph
{
private:

  struct Vertex {
    string name;
    vector<int> adj;
    int vertex_num;

    friend ostream& operator<<(ostream& output, Vertex const& vertex) {
      if (vertex.name.empty()) {
        return output << vertex.vertex_num;
      } else {
        return output << vertex.name;
      }
    }
  };

  vector<Vertex> vertices;
  int V;
  bool directed;


public:
  Graph(int V, bool directed) : V(V), directed(directed) {
    vertices.resize(V);
    for(int u=0; u < V; u++) {
      vertices[u].vertex_num = u;
    }
  }

  Graph(const Graph& rhs) {
    V = rhs.V;
    directed = rhs.directed;
    vertices.resize(V);

    for (int u=0; u < V; u++ ) {
        for (auto x: vertices[u].adj) {
            addDirectedEdge(u, x);
        }
    }

  }

int numVertices() {
  return V;
}

void addDirectedEdge(int u, int v)
{
  if (find(vertices[u].adj.begin(), vertices[u].adj.end(), v) == vertices[u].adj.end())
    vertices[u].adj.push_back(v);
}

void addEdge(int u, int v)
{
  addDirectedEdge(u,v);

  if (!directed)
    addDirectedEdge(v, u);
}

void printGraph(bool strict)
{
    if (strict)
        cout << "strict ";
    
    std::string connector;
    if (directed) {
      cout << "digraph {" << endl;
      connector = "->";
    }
    else {
      cout << "graph {" << endl;
      connector = "--";
    }

    for (auto x : vertices) {
      for (auto y : x.adj) {
        cout << " " << x << " " << connector << " " << y << endl;
      }
    }
    cout << "}" << endl;
}

}; // Graph
