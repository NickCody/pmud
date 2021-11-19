#pragma once

#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

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
      vertices[u].vertex_num = u+1;
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

void printDotGraph(bool strict)
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

void printStructure() {
    std::string connector = "--";

    for (auto x : vertices) {
      cout << x;
      for (auto y : x.adj) {
        cout << " " << connector << " " << y;
      }
      cout << endl;
    }
}

void printMatrix() {
  vector<vector<int>> rows;
  for (size_t i=0; i < vertices.size(); i++) {
    rows.push_back(vector<int>(vertices.size()));
    for (size_t j=0; j < vertices.size(); j++) {
      rows[i][j] = 0;
    }
   }

  for (size_t i=0; i < vertices.size(); i++) {
    for (int x : vertices[i].adj) {
      rows[i][vertices[x].vertex_num-1] = 1;
    }
  }

  cout << "      ";

  for (size_t j=0; j < vertices.size(); j++) {
    cout << setw(3) << j << " ";
  }

  cout << endl;

  cout << "     ";

  for (size_t j=0; j < vertices.size(); j++) {
    cout << "----";
  }

  cout << "-";

  cout << endl;


  for (size_t i=0; i < vertices.size(); i++) {
    cout << setw(3) << i << " | ";
    for (size_t j=0; j < vertices.size(); j++) {
      cout << setw(3) << rows[i][j] << " ";
    }
    cout << endl;
  }

}

}; // Graph
