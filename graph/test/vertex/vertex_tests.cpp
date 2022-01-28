#include "graph/common.h"
#include "gtest/gtest.h"

using namespace cody::graph;

TEST(VertexCreationTest, ShouldAssignAdjancencyIndices) {
  Graph<> graph(3, GRAPHTYPE_DIRECTED);
  graph.addEdge(0, 1);
  graph.addEdge(1, 0);
  graph.addEdge(1, 1);
  graph.addEdge(1, 2);

  EXPECT_EQ(graph.numVertices(), 3);
  EXPECT_EQ(graph[graph[0][0]].index(), 1);
  EXPECT_EQ(graph[graph[1][0]].index(), 0);
  EXPECT_EQ(graph[graph[1][1]].index(), 1);
  EXPECT_EQ(graph[graph[1][2]].index(), 2);
}
