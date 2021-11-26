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

void addEdgesRandom(Graph<>& graph, int N) {
    for (int i=0; i < N; i++) {
        graph.addEdge(rand() % graph.getNumVertices(), rand() % graph.getNumVertices());
    }
}

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

    Graph<> graph(V, true);
    addEdgesRandom(graph, R);

    printMatrix(graph);

    BFS::SearchData data = BFS::BreadthFirstSearch(graph, 0);
    int s = 0;
    int v = V-1;
    cout << endl;
    cout << "Path from " << graph.getVertex(s) << " to " << graph.getVertex(v) << " is: ";
    printPath(graph, s, v, data.π);

    return 0;
}