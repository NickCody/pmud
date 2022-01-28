#pragma once

#include <vector>
#include <memory>
#include <array>
#include <string>
#include <algorithm>
#include <iomanip>
#include <optional>

using namespace std;

namespace cody {
  namespace graph {

    template <typename D> struct Vertex {
    private:
      using adj_t = std::vector<int>;

      int _index = 0; // range(0..V-1)
      std::optional<D> data = {};
      adj_t _adj;

    public:
      Vertex(int index, std::optional<D> data = nullopt)
          : _index(index),
            data(data) {
      }

      Vertex() = delete;

      size_t adjSize() const {
        return _adj.size();
      }

      int index() const {
        return _index;
      }

      const int operator[](size_t v) const {
        return _adj[v];
      }

      void addAdj(int v) {
        _adj.push_back(v);
      }

      void clearAdj() {
        _adj.clear();
      }

      using iterator = adj_t::iterator;
      using const_iterator = adj_t::const_iterator;

      iterator begin() {
        return _adj.begin();
      }

      iterator end() {
        return _adj.end();
      }

      const_iterator begin() const {
        return _adj.begin();
      }

      const_iterator end() const {
        return _adj.end();
      }

      const_iterator cbegin() const {
        return _adj.cbegin();
      }

      const_iterator cend() const {
        return _adj.cend();
      }

      friend ostream& operator<<(ostream& output, Vertex const& vertex) {
        output << vertex.data.value_or(vertex.index());
        return output;
      }
    };

  } // namespace graph
} // namespace cody