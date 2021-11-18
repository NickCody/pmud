# cpp-game

A collection of simple games and experiments written in C++, ncurses, graphviz and built by bazel.

## Projects

### life

```bash
bazel run //life:binary -- -c 100 -r 50 -g 10000 -d 100
```

###  graph

```bash
bazel run //graph:binary | xdot -
```