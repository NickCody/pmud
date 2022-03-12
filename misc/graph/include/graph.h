#pragma once

#include <memory>
#include <array>
#include <string>
#include <algorithm>
#include <iomanip>
#include <optional>

#include "vertex.h"

namespace cody {
  namespace graph {

    using namespace std;

    enum GraphType { GRAPHTYPE_DIRECTED, GRAPHTYPE_UNDIRECTED };

    template <typename VertexType = Vertex<int>> class Graph {
    private:
      using vertices_t = vector<VertexType>;

      vertices_t vertices;
      int V;
      GraphType directed;

    public:
      Graph(int numVertices, GraphType directed)
          : V(numVertices),
            directed(directed) {
        for (int u = 0; u < V; u++) {
          vertices.push_back(VertexType(u));
        }
      }

      Graph(const Graph& rhs) {
        V = rhs.V;
        directed = rhs.directed;
        for (int u = 0; u < V; u++) {
          vertices.push_back(VertexType(u));
        }

        for (auto const& x : rhs) {
          for (int y : x) {
            addDirectedEdge(x.index(), y);
          }
        }
      }

      Graph& operator=(Graph&& other) {
        vertices = std::move(other.vertices);
        directed = other.directed;
        V = other.V;
        return *this;
      }

      using iterator = vertices_t::iterator;
      using const_iterator = vertices_t::const_iterator;

      iterator begin() {
        return vertices.begin();
      }

      iterator end() {
        return vertices.end();
      }

      const_iterator begin() const {
        return vertices.begin();
      }

      const_iterator end() const {
        return vertices.end();
      }

      const_iterator cbegin() const {
        return vertices.cbegin();
      }

      const_iterator cend() const {
        return vertices.cend();
      }

      int numVertices() const {
        return V;
      }

      void addDirectedEdge(int u, int v) {
        auto& x = vertices[u];

        if (find(x.begin(), x.end(), v) == x.end())
          x.addAdj(v);
      }

      void addEdge(int u, int v) {
        addDirectedEdge(u, v);

        if (directed == GRAPHTYPE_UNDIRECTED)
          addDirectedEdge(v, u);
      }

      const VertexType& vertex(int u) const {
        return vertices[u];
      }

      const VertexType& operator[](size_t u) const {
        return vertices[u];
      }

      bool isDirected() const {
        return directed == GRAPHTYPE_DIRECTED;
      }

      Graph transpose() {

        Graph _temp(*this);

        if (directed == GRAPHTYPE_UNDIRECTED)
          return _temp;

        for (VertexType& x : _temp.vertices) {
          x.clearAdj();
        }

        for (auto const& x : vertices) {
          for (int y : x) {
            _temp.addDirectedEdge(y, x.index());
          }
        }

        return _temp;
      }

    }; // Graph

  } // namespace graph
} // namespace cody