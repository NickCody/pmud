#pragma once

#include <limits.h>
#include <queue>
#include <memory>

#include "graph.h"

namespace cody {
namespace graph {
namespace DFS {

using namespace std;

enum SearchColor { WHITE, GRAY, BLACK };

struct SearchData {
  vector<int> pi;
  vector<SearchColor> color;
  vector<int> d;
  vector<int> f;
  weak_ptr<Graph<>> pGraph;

  friend ostream& operator<<(ostream& output, SearchData const& data) {
    int V = data.pi.size();

    if (auto pGraph = data.pGraph.lock()) {
      output << " d: ";
      for(int x=0; x < V; x++) {
        output << "(" << setw(3) << pGraph->getVertex(x) << "," << setw(3) << data.d[x] << ") ";
      }
      output << endl;

      output << " f: ";
      for(int x=0; x < V; x++) {
        output << "(" << setw(3) << pGraph->getVertex(x) << "," << setw(3) << data.f[x] << ") ";
      }
      output << endl;

      output << "pi: ";
      for(int x=0; x < V; x++) {
        int pred;
        if (data.pi[x] < 0)
            pred = data.pi[x];
        else
            pred = data.pi[x]+1;
        output <<  "(" << setw(3) << pGraph->getVertex(x) << "," << setw(3) << pred << ") ";
      }
    } else {
      output << "graph is no longer available." << endl;
    }

    return output;
  }

};

void DFS_Visit(Graph<>& graph, int u, int& time, vector<int>& π, vector<SearchColor>& color, vector<int>& d, vector<int>& f ) {
  color[u] = GRAY;
  d[u] = ++time;
  
  for(auto const& v : graph.getVertex(u)) {
    if (color[v] == WHITE) {
      π[v] = u;
      DFS_Visit(graph, v, time, π, color, d, f);
    }
  }

  color[u] = BLACK;
  f[u] = ++time;
}

SearchData DepthFirstSearch(weak_ptr<Graph<>> graph) {
  
  if (auto g = graph.lock()) {
    int V = g->getNumVertices();

    vector<int> π(V);
    vector<SearchColor> color(V);
    vector<int> d(V);
    vector<int> f(V);

    int time = 0;

    for(int u=0; u < V; u++ ) {
      color[u] = WHITE;
      π[u] = -1;
      d[u] = -1;
      f[u] = -1;
    }

    for(int u=0; u < V; u++ ) {
      if (color[u] == WHITE) {
        DFS_Visit(*g, u, time, π, color, d, f);
      }
    }

    return SearchData { π, color, d, f, graph };
  } else {
    throw runtime_error("graph is null");
  }
}
    

} // namespace DFS
} // namespace graph
} // namesapce cody