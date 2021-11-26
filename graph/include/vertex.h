#pragma once

#include <memory>
#include <array>
#include <string>
#include <algorithm>
#include <iomanip>
#include <optional>

using namespace std;

namespace cody {
namespace graph {

template <typename D>
struct Vertex {
  private:
    using adj_t = std::vector<int>;

    std::optional<D> data = {};
    int vertex_num = 0; // range(1..N)
    adj_t adj;
    
  public:

    size_t getAdjSize() const { return adj.size(); }
    int getVertexNum() const { return vertex_num; }
    int getAdj(int v) const { return adj[v]; }
    
    void setVertexNum(int v) { vertex_num = v; }
    void addAdj(int v) { adj.push_back(v); }
    void clearAdj() { adj.clear(); }

    using iterator = adj_t::iterator;
    using const_iterator = adj_t::const_iterator;

    iterator begin() { return adj.begin(); }
    iterator end() { return adj.end(); }
    const_iterator begin() const { return adj.begin(); }
    const_iterator end() const { return adj.end(); }
    const_iterator cbegin() const { return adj.cbegin(); }
    const_iterator cend() const { return adj.cend(); }

    friend ostream& operator<<(ostream& output, Vertex const& vertex) {
      output << vertex.data.value_or(vertex.vertex_num);
      return output;
    }

};


} // namespace graph
} // namespace cody