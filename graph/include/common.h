#pragma once

#include <limits.h>
#include "graph.h"

namespace cody {
namespace graph {

const string PRESENT = "*";
const string ABSENT = " ";

const int INVALID_VERTEX = -1;

enum DotGraphType { DOT_STRICT, DOT_NONSTRICT };

void _printPath(Graph<>& graph, int s, int v, vector<int>& π) {
  if (v == s) {
    cout << graph.getVertex(v);
  } else {
    if (π[v] == INVALID_VERTEX) {
      cout << "No path from " << graph.getVertex(s) << " to " << graph.getVertex(v) << " exists." << endl;
    } else {
      _printPath(graph, s, π[v], π);
      cout << " -> " << graph.getVertex(v);
    }
  }
}

void printPath(Graph<>& graph, int s, int v, vector<int>& π) {
  _printPath(graph, s, v, π);
  cout << endl;
}


void printDot(Graph<>& graph, DotGraphType strict)
{
    if (strict == DOT_STRICT)
        cout << "strict ";
    
    std::string connector;
    if (graph.isDirected()) {
      cout << "digraph {" << endl;
      connector = "->";
    }
    else {
      cout << "graph {" << endl;
      connector = "--";
    }

    for (auto const &x : graph) {
      cout << " " << x;
      for (int y : x) {
        cout << " " << connector << " " << graph.getVertex(y);
      }
      cout << endl;
    }
    cout << "}" << endl;
}

void printStructure(Graph<>& graph) {
  std::string connector = "--";

  for (auto const &x : graph) {
    cout << x;
    for (int y : x) {
      cout << " " << connector << " " << graph.getVertex(y);
    }
    cout << endl;
  }
}

void printMatrix(Graph<>& graph) {
  vector<vector<string>> rows;
  const size_t V = graph.getNumVertices();

  for (size_t i=0; i < V; i++) {
    rows.push_back(vector<string>());
    for (size_t j=0; j < V; j++) {
      rows[i].push_back(ABSENT);
    }
  }

  for (auto const &x : graph) {
    int u=x.getVertexNum()-1;
    for (int y : x) {
      int v = graph.getVertex(y).getVertexNum()-1;
      rows[u][v] = PRESENT;
    }
  }

  cout << "      ";

  for (size_t j=0; j < V; j++) {
    cout << setw(3) << j+1 << " ";
  }

  cout << endl;

  cout << "     ";

  for (size_t j=0; j <V; j++) {
    cout << "----";
  }

  cout << "-";

  cout << endl;

  for (size_t i=0; i < V; i++) {
    cout << setw(3) << i+1 << " | ";
    for (size_t j=0; j < V; j++) {
      cout << "  " << rows[i][j] << " ";
    }
    cout << endl;
  }

}

void addEdgesRandom(Graph<>& graph, int N) {
    for (int i=0; i < N; i++) {
        graph.addEdge(rand() % graph.getNumVertices(), rand() % graph.getNumVertices());
    }
}

void addEdgesAllConnect(Graph<>& graph) {
    int N = graph.getNumVertices();
    for (int i=0; i < N; i++) {
        for (int j=i+1; j < N; j++) {
            if (i != j)
                graph.addEdge(i, j);
        }
    }
}


} // namespace graph
} // namespace cody