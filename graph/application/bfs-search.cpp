#include <iostream>
#include <vector>
#include <clocale>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#include "graph/graph.h"
#include "graph/common.h"
#include "graph/BFS.h"

using namespace cody::graph;

int main(int argc, char** argv)
{
    std::setlocale(LC_ALL, "en_US.UTF-8");
    srand(time(0));

    int V=20;
    int R=V*10;

    if (argc == 2) {
        V = atoi(argv[1]);
        R = V*10;
    } else if (argc == 3) {
        V = atoi(argv[1]);
        R = atoi(argv[2]);
    }

    auto graph = make_shared<Graph<>>(V, GRAPHTYPE_DIRECTED);
    
    addEdgesRandom(*graph, R);
    printMatrix(*graph);

    BFS::SearchData data = BFS::BreadthFirstSearch(graph, 0);
    int s = 0;
    int v = V-1;
    cout << endl;
    cout << "Path from " << graph->vertex(s) << " to " << graph->vertex(v) << " is: ";
    printPath(*graph, s, v, data.pi);

    return 0;
}