#pragma once

#include <limits.h>
#include <queue>

#include "common.h"

namespace cody {
namespace graph {
namespace BFS {

using namespace std;

enum SearchColor { WHITE, GRAY, BLACK };

struct SearchData {
  vector<int> pi;
  vector<SearchColor> color;
  vector<int> d;
  weak_ptr<Graph<>> graph;
};

// Introduction to Algorithms, by Cormen, Leiserson, and Rivest
//
SearchData BreadthFirstSearch(weak_ptr<Graph<>> graph, int s) {
  
  if (auto g = graph.lock()) {
    int V = g->getNumVertices();

    vector<int> π(V);
    vector<SearchColor> color(V);
    vector<int> d(V);

    queue<int> Q;

    for(int u=0; u < V; u++ ) {
      if (u == s)
        continue;

      color[u] = WHITE;
      d[u] = INT_MAX;
      π[u] = INVALID_VERTEX;
    }

    color[s] = GRAY;
    d[s] = 0;
    π[s] = INVALID_VERTEX;
    Q.push(s);

    while(!Q.empty()) {
      int& u = Q.front();
      for(int v : g->getVertex(u)) {
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

    return SearchData { π, color, d, graph };
  } else {
    throw runtime_error("graph is null");
  }
}


} // namespace BFS
} // namespace graph
} // namesapce cody