# cpp-game

A collection of simple games and experiments written in C++, ncurses, graphviz and built by bazel.

## Projects

### life

```bash
bazel run //life:binary -- -c 100 -r 50 -g 10000 -d 100
```

###  graph

```bash
bazel run //graph/application:fully-connected -- 10           # complete graph (dot output)
bazel run //graph/application:random-connected --  10 100     # randomly connected (dot output)
bazel run //graph/application:matrix -- 10 100                # prints adjacency matrix representation (custom output)
bazel run //graph/application:structure --  10 100            # prints adj-list structure (custom output)
bazel run //graph/application:bfs-search --  10 100           # Does a BFS from vertex 1 -> n
```

### util

```
bazel run //util:cppversion
```

## TODO

Unified CLI so for any program, you can choose output format (matrix, structure, dot).