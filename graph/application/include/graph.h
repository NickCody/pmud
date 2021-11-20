#pragma once

#include <memory>
#include <array>
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

  string PRESENT = "×";
  string ABSENT = " ";

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
    for(int u=0; u < V; u++) {
      vertices[u].vertex_num = u+1;
    }

    for (int u=0; u < V; u++ ) {
        for (auto x: rhs.vertices[u].adj) {
            addDirectedEdge(u, x);
        }
    }

  }

  Graph& operator=(Graph&& other) {
    cout << "std::move() called" << endl;
      vertices = std::move(other.vertices);
      directed = other.directed;
      V = other.V;
      return *this;
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

  Graph transpose() {

    Graph _temp(*this);

    if (!directed)
      return _temp;

    for (Vertex& x : _temp.vertices) {
      x.adj.clear();
    }

    for (Vertex& x : vertices) {
      for (int y : x.adj) {
        _temp.addDirectedEdge(y, x.vertex_num-1);
      }
    }

    return _temp;
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
    vector<vector<string>> rows;
    for (size_t i=0; i < vertices.size(); i++) {
      rows.push_back(vector<string>(vertices.size()));
      for (size_t j=0; j < vertices.size(); j++) {
        rows[i][j] = ABSENT;
      }
    }

    for (size_t i=0; i < vertices.size(); i++) {
      for (int x : vertices[i].adj) {
        rows[i][vertices[x].vertex_num-1] = PRESENT;
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
        cout << "  " << rows[i][j] << " ";
      }
      cout << endl;
    }

  }

}; // Graph
