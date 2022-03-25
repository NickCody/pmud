#include "graph/common.h"
#include "gtest/gtest.h"

using namespace cody::graph;

TEST(GraphCreationTest, DirectedGraphShouldHaveSpecifiedAdjacencies) {
  Graph<> graph(2, GRAPHTYPE_DIRECTED);
  graph.addEdge(0, 1);

  EXPECT_EQ(graph.numVertices(), 2);
  EXPECT_EQ(graph[0].adjSize(), 1);
  EXPECT_EQ(graph[1].adjSize(), 0);

  EXPECT_EQ(graph[0][0], 1);
}

TEST(GraphCreationTest, UndirectedGraphShouldHaveSpecifiedAdjacencies) {
  Graph<> graph(2, GRAPHTYPE_UNDIRECTED);
  graph.addEdge(0, 1);

  EXPECT_EQ(graph.numVertices(), 2);
  EXPECT_EQ(graph[0].adjSize(), 1);
  EXPECT_EQ(graph[1].adjSize(), 1);

  EXPECT_EQ(graph[0][0], 1);
  EXPECT_EQ(graph[1][0], 0);
}

TEST(GraphCreationTest, DirectedGraphAssignmenShouldCloneGraph) {
  Graph<> graph(3, GRAPHTYPE_DIRECTED);
  graph.addEdge(0, 1);
  graph.addEdge(1, 0);
  graph.addEdge(1, 1);
  graph.addEdge(1, 2);

  Graph<> graph_cloned(graph);

  EXPECT_EQ(graph_cloned.numVertices(), 3);
  EXPECT_EQ(graph_cloned[graph_cloned[0][0]].index(), 1);
  EXPECT_EQ(graph_cloned[graph_cloned[1][0]].index(), 0);
  EXPECT_EQ(graph_cloned[graph_cloned[1][1]].index(), 1);
  EXPECT_EQ(graph_cloned[graph_cloned[1][2]].index(), 2);
}

TEST(GraphCreationTest, UnDirectedGraphAssignmentShouldCloneGraph) {
  Graph<> graph(3, GRAPHTYPE_UNDIRECTED);
  graph.addEdge(0, 1);
  graph.addEdge(1, 0);
  graph.addEdge(1, 1);
  graph.addEdge(1, 2);

  Graph<> graph_cloned(graph);

  EXPECT_EQ(graph_cloned.numVertices(), 3);
  EXPECT_EQ(graph_cloned[graph_cloned[0][0]].index(), 1);
  EXPECT_EQ(graph_cloned[graph_cloned[1][0]].index(), 0);
  EXPECT_EQ(graph_cloned[graph_cloned[1][1]].index(), 1);
  EXPECT_EQ(graph_cloned[graph_cloned[1][2]].index(), 2);
}

TEST(GraphCreationTest, TransposedGraphShouldTranposeGraph) {
  Graph<> graph(3, GRAPHTYPE_DIRECTED);
  graph.addEdge(0, 1); // transposed 1 -- 2
  graph.addEdge(1, 0); // transposed 0 -- 1
  graph.addEdge(1, 2); // transposed 2 -- 1

  Graph<> graph_transposed = graph.transpose();

  EXPECT_EQ(graph.numVertices(), 3);
  EXPECT_EQ(graph_transposed.numVertices(), 3);

  EXPECT_EQ(graph[0].adjSize(), 1);
  EXPECT_EQ(graph_transposed[0].adjSize(), 1);

  EXPECT_EQ(graph[1].adjSize(), 2);
  EXPECT_EQ(graph_transposed[1].adjSize(), 1);

  EXPECT_EQ(graph[2].adjSize(), 0);
  EXPECT_EQ(graph_transposed[2].adjSize(), 1);
}
