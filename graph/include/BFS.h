#pragma once

#include <limits.h>
#include <queue>

#include "graph.h"

namespace cody {
namespace graph {
namespace BFS {

using namespace std;

enum SearchColor { WHITE, GRAY, BLACK };

struct SearchData {
  vector<int> π;
  vector<SearchColor> color;
  vector<int> d;
};

// Introduction to Algorithms, by Cormen, Leiserson, and Rivest
//
SearchData BreadthFirstSearch(Graph<>& graph, int s) {
  
  vector<int> π(graph.getNumVertices());
  vector<SearchColor> color(graph.getNumVertices());
  vector<int> d(graph.getNumVertices());

  queue<int> Q;

  for(int u=0; u < graph.getNumVertices(); u++ ) {
    if (u == s)
      continue;

    color[u] = WHITE;
    d[u] = INT_MAX;
    π[u] = INT_MIN;
  }

  color[s] = GRAY;
  d[s] = 0;
  π[s] = INT_MIN;
  Q.push(s);

  while(!Q.empty()) {
    int& u = Q.front();
    for(size_t i = 0; i < graph.getVertex(u).getAdjSize(); i++) {
      int v = graph.getVertex(u).getAdj(i);
      if (color[v] == WHITE) {
        color[v] = GRAY;
        d[v] = d[u] + 1;
        π[v] = u;
        Q.push(v);
      }
    }
    Q.pop();
    color[u] = BLACK;
  }

  return SearchData { π, color, d };
}


} // namespace BFS
} // namespace graph
} // namesapce cody