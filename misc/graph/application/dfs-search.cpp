#include <iostream>
#include <vector>
#include <clocale>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#include "graph/common.h"
#include "graph/DFS.h"

using namespace cody::graph;

int main(int argc, char** argv) {
  std::setlocale(LC_ALL, "en_US.UTF-8");
  srand(11);

  int V = 10;
  int R = V * 10;

  if (argc == 2) {
    V = atoi(argv[1]);
    R = V * 10;
  } else if (argc == 3) {
    V = atoi(argv[1]);
    R = atoi(argv[2]);
  }

  auto graph = make_shared<Graph<>>(V, GRAPHTYPE_DIRECTED);

  addEdgesRandom(*graph, R);

  DFS::SearchData data = DFS::DepthFirstSearch(graph, 4);

  cerr << "Performed DFS on the graph:" << endl;
  printStructure(*graph, cerr);
  cerr << "With the following search/traversal data:" << endl;
  cerr << data << endl;

  printDotDfsAnnotate(*graph, DOT_STRICT, data);

  return 0;
}