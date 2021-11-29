#include "graph/common.h"
#include "gtest/gtest.h"

using namespace cody::graph;

TEST(GraphCreationTest, HaveSpecifiedAdjacencies) {
    Graph<> graph(2, GRAPHTYPE_DIRECTED);
    graph.addDirectedEdge(0, 1);

    EXPECT_EQ(graph.numVertices(), 2);
    EXPECT_EQ(graph[0].adjSize(), 1);
    EXPECT_EQ(graph[1].adjSize(), 0);
}