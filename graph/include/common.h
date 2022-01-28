#pragma once

#include <iostream>
#include <limits.h>
#include "graph.h"

namespace cody {
  namespace graph {

    const string PRESENT = "*";
    const string ABSENT = " ";

    const int INVALID_VERTEX = -1;

    enum DotGraphType { DOT_STRICT, DOT_NONSTRICT };

    void _printPath(Graph<>& graph, int s, int v, vector<int>& π, ostream& output = cout) {
      if (v == s) {
        output << graph[v];
      } else {
        if (π[v] == INVALID_VERTEX) {
          output << "No path from " << graph[s] << " to " << graph[v] << " exists." << endl;
        } else {
          _printPath(graph, s, π[v], π);
          output << " -> " << graph[v];
        }
      }
    }

    void printPath(Graph<>& graph, int s, int v, vector<int>& π, ostream& output = cout) {
      _printPath(graph, s, v, π, output);
      output << endl;
    }

    void printDot(Graph<>& graph, DotGraphType strict, ostream& output = cout) {
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
        for (int y : x) {
          output << " " << x << " " << connector << " " << graph[y] << endl;
        }
      }
      output << "}" << endl;
    }

    void printStructure(Graph<>& graph, ostream& output = cout) {
      std::string connector = "--";

      for (auto const& x : graph) {
        output << x;
        for (int y : x) {
          output << " " << connector << " " << graph[y];
        }
        output << endl;
      }
    }

    void printMatrix(Graph<>& graph, ostream& output = cout) {
      vector<vector<string>> rows;
      const size_t V = graph.numVertices();

      // init strings
      for (size_t i = 0; i < V; i++) {
        rows.push_back(vector<string>());
        for (size_t j = 0; j < V; j++) {
          rows[i].push_back(ABSENT);
        }
      }

      // populate strings
      for (auto const& x : graph) {
        int u = x.index();
        for (int y : x) {
          int v = graph[y].index();
          rows[u][v] = PRESENT;
        }
      }

      // indent header
      output << "      ";

      // print header
      for (size_t j = 0; j < V; j++) {
        output << setw(3) << graph[j] << " ";
      }

      output << endl;

      // index border
      output << "    +";

      // print top border
      for (size_t j = 0; j < V; j++) {
        output << "----";
      }

      output << "-" << endl;

      // print grid
      for (size_t i = 0; i < V; i++) {
        output << setw(3) << graph[i] << " | ";
        for (size_t j = 0; j < V; j++) {
          output << "  " << rows[i][j] << " ";
        }
        output << endl;
      }
    }

    void addEdgesRandom(Graph<>& graph, int N) {
      for (int i = 0; i < N; i++) {
        graph.addEdge(rand() % graph.numVertices(), rand() % graph.numVertices());
      }
    }

    void addEdgesAllConnect(Graph<>& graph) {
      int N = graph.numVertices();
      for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
          if (i != j)
            graph.addEdge(i, j);
        }
      }
    }

  } // namespace graph
} // namespace cody