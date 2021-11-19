#include <iostream>
#include <vector>
#include <clocale>
#include <stdlib.h>
#include <time.h>

#include "graph.h"

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

    int V=5;

    if (argc == 2)
        V = atoi(argv[1]);

    Graph graph(V, false);
    addEdgesAllConnect(graph);
    graph.printDotGraph(true);

    return 0;
}