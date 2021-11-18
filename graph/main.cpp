#include <iostream>
#include <vector>
#include <clocale>
#include <stdlib.h>
#include <time.h>

#include "graph.h"

void addEdges1(Graph& graph) {
    graph.addDirectedEdge(0, 1);
    graph.addDirectedEdge(0, 4);
    graph.addDirectedEdge(1, 2);
    graph.addDirectedEdge(1, 3);
    graph.addDirectedEdge(1, 4);
    graph.addDirectedEdge(2, 3);
    graph.addDirectedEdge(3, 4);
}

void addEdges2(Graph& graph) {
    graph.addEdge(0, 1);
}

void addEdges3(Graph& graph) {
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 2);
    //graph.addEdge(2, 0);
}

void addEdgesRandom(Graph& graph, int N) {
    for (int i=0; i < N; i++) {
        graph.addEdge(rand() % graph.numVertices(), rand() % graph.numVertices());
    }
}

void addEdgesAllConnect(Graph& graph) {
    int N = graph.numVertices();
    for (int i=0; i < N; i++) {
        for (int j=i+1; j < N; j++) {
            if (i != j)
                graph.addEdge(i, j);
        }
    }
}

int main(int argc, char** argv)
{
    std::setlocale(LC_ALL, "en_US.UTF-8");
    srand(time(0));

    Graph graph(20, false);
    addEdgesAllConnect(graph);
    graph.printGraph(true);

    return 0;
}