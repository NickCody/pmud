#include <iostream>
#include <vector>
#include <clocale>
#include <stdlib.h>
#include <time.h>
#include "graph/common.h"

using namespace cody::graph;

int main(int argc, char** argv) {
  std::setlocale(LC_ALL, "en_US.UTF-8");

  int V = 5;

  if (argc == 2)
    V = atoi(argv[1]);

  Graph<> graph(V, GRAPHTYPE_UNDIRECTED);
  addEdgesAllConnect(graph);
  printDot(graph, DOT_STRICT);

  return 0;
}