#pragma once

#include <limits.h>
#include <queue>
#include <graph.h>

namespace cody {
namespace graph {

using namespace std;

enum SearchColor { WHITE, GRAY, BLACK };

struct SearchData {

  vector<int> π;
  vector<SearchColor> color;
  vector<int> d;

};

SearchData BFS(Graph<>& graph, int s) {
  
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
    for(size_t i = 0; i < graph.getAdjancency(u).size(); i++) {
      int v = graph.getVertex(u).adj[i];
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

void _printPath(Graph<>& graph, int s, int v, SearchData& data) {
  if (v == s) {
    cout << graph.getVertex(v);
  } else {
    if (data.π[v] == INT_MIN) {
      cout << "No path from " << graph.getVertex(s) << " to " << graph.getVertex(v) << " exists." << endl;
    } else {
      _printPath(graph, s, data.π[v], data);
      cout << " -> " << graph.getVertex(v);
    }
  }
}

void printPath(Graph<>& graph, int s, int v, SearchData& data) {
  _printPath(graph, s, v, data);
  cout << endl;
}


} // namespace graph
} // namesapce cody