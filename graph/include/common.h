#pragma once

#include "graph.h"

namespace cody {
namespace graph {

void _printPath(Graph<>& graph, int s, int v, vector<int>& π) {
  if (v == s) {
    cout << graph.getVertex(v);
  } else {
    if (π[v] == INT_MIN) {
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

} // namespace graph
} // namespace cody