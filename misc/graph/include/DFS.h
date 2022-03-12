#pragma once

#include <limits.h>
#include <queue>
#include <memory>
#include <cstdlib>
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

        static vector<tuple<int, int>> sort_by_discovery(vector<int> source) {
          vector<tuple<int, int>> sorted;
          for (size_t i = 0; i < source.size(); i++) {
            sorted.push_back(make_tuple(i, source[i]));
          }

          sort(sorted.begin(), sorted.end(), [](tuple<int, int> a, tuple<int, int> b) { return std::get<1>(a) < std::get<1>(b); });

          return sorted;
        }

        static void print_data_vec(ostream& output, Graph<>& graph, vector<tuple<int, int>>&& tv) {
          for (auto t : tv) {
            output << "(" << setw(3) << graph[std::get<0>(t)] << "," << setw(3) << std::get<1>(t) << ") ";
          }
        }

        friend ostream& operator<<(ostream& output, SearchData const& data) {
          if (auto pGraph = data.pGraph.lock()) {
            output << " d: ";
            print_data_vec(output, *pGraph, sort_by_discovery(data.d));
            output << endl;

            output << " f: ";
            print_data_vec(output, *pGraph, sort_by_discovery(data.f));
            output << endl;

            output << "pi: ";
            for (auto x : *pGraph) {
              output << "(" << setw(3) << x << "," << setw(3) << data.pi[x.index()] << ") ";
            }
          } else {
            output << "graph is no longer available." << endl;
          }

          return output;
        }
      };

      void DFS_Visit(Graph<>& graph, int u, int& time, vector<int>& π, vector<SearchColor>& color, vector<int>& d, vector<int>& f) {
        color[u] = GRAY;
        d[u] = time++;

        for (int v : graph[u]) {
          if (color[v] == WHITE) {
            π[v] = u;
            DFS_Visit(graph, v, time, π, color, d, f);
          }
        }

        color[u] = BLACK;
        f[u] = time++;
      }

      SearchData DepthFirstSearch(weak_ptr<Graph<>> graph, int s) {

        if (auto g = graph.lock()) {
          int V = g->numVertices();

          vector<int> π(V);
          vector<SearchColor> color(V);
          vector<int> d(V);
          vector<int> f(V);

          int time = 0;

          for (int u = 0; u < V; u++) {
            color[u] = WHITE;
            π[u] = -1;
            d[u] = -1;
            f[u] = -1;
          }

          // visit s first
          DFS_Visit(*g, s, time, π, color, d, f);

          // visit remainder
          for (int u = 0; u < V; u++) {
            if (u == s)
              continue;

            if (color[u] == WHITE) {
              DFS_Visit(*g, u, time, π, color, d, f);
            }
          }

          return SearchData{ π, color, d, f, graph };
        } else {
          throw runtime_error("graph is null");
        }
      }

      void printDotDfsAnnotate(Graph<>& graph, DotGraphType strict, SearchData& data, ostream& output = cout) {
        if (strict == DOT_STRICT)
          output << "strict ";

        std::string connector;
        if (graph.isDirected()) {
          output << "digraph {" << endl;
          connector = "->";
        } else {
          output << "graph {" << endl;
          connector = "--";
        }

        for (auto const& x : graph) {
          output << "  " << x << " [xlabel=\"(" << data.d[x.index()] << ".." << data.f[x.index()] << ")\" fontsize=\"8\"]" << endl;
        }

        for (auto const& x : graph) {
          for (int y : x) {
            output << "  " << x << " " << connector << " " << graph[y] << endl;
          }
        }
        output << "}" << endl;
      }

    } // namespace DFS
  }   // namespace graph
} // namespace cody