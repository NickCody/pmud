#include <iostream>
#include <vector>
#include <clocale>
#include <stdlib.h>
#include <time.h>
#include "graph/common.h"

using namespace cody::graph;

int main(int argc, char** argv)
{
    std::setlocale(LC_ALL, "en_US.UTF-8");

    int V=5;

    if (argc == 2)
        V = atoi(argv[1]);

    Graph<> graph(V, false);
    addEdgesAllConnect(graph);
    printDotGraph(graph, true);

    return 0;
}