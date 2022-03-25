#include <iostream>
#include <vector>
#include <clocale>
#include <stdlib.h>
#include <time.h>

#include "graph/common.h"

using namespace cody::graph;

int main(int argc, char** argv) {
  std::setlocale(LC_ALL, "en_US.UTF-8");
  srand(time(0));

  int V = 5;
  int R = V * 10;

  if (argc == 2) {
    V = atoi(argv[1]);
    R = V * 10;
  } else if (argc == 3) {
    V = atoi(argv[1]);
    R = atoi(argv[2]);
  }

  Graph<> graph(V, GRAPHTYPE_DIRECTED);
  addEdgesRandom(graph, R);
  printMatrix(graph);
  Graph transposed = graph.transpose();
  printMatrix(transposed);

  return 0;
}