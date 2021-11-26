#pragma once

#include <limits.h>
#include <queue>

#include "graph.h"

namespace cody {
namespace graph {
namespace DFS {

using namespace std;

enum SearchColor { WHITE, GRAY, BLACK };

struct SearchData {
  vector<int> π;
  vector<SearchColor> color;
  vector<int> d;
  vector<int> f;
};

void DFS_Visit(Graph<>& graph, int u, int& time, vector<int> π, vector<SearchColor> color, vector<int> d, vector<int> f ) {
  color[u] = GRAY;
  d[u] = time++;
  for(size_t v = 0; v < graph.getAdjancency(u).size(); v++) {
    if (color[v] == WHITE) {
      π[v] = u;
      DFS_Visit(graph, v, time, π, color, d, f);
    }
  }

  color[u] = BLACK;
  f[u] = time++;
}

SearchData DepthFirstSearch(Graph<>& graph) {
  
  vector<int> π(graph.getNumVertices());
  vector<SearchColor> color(graph.getNumVertices());
  vector<int> d(graph.getNumVertices());
  vector<int> f(graph.getNumVertices());

  int time = 0;

  for(int u=0; u < graph.getNumVertices(); u++ ) {
    color[u] = WHITE;
    π[u] = INT_MIN;
  }

  for(int u=0; u < graph.getNumVertices(); u++ ) {
    if (color[u] == WHITE) {
      DFS_Visit(graph, u, time, π, color, d, f);
    }
  }

  return SearchData { π, color, d, f };
}
    

} // namespace DFS
} // namespace graph
} // namesapce cody