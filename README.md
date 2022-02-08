# cpp-game

A collection of simple games and experiments written in C++, ncurses, graphviz and built by bazel.

## Setup

1. Make sure vbscode and docker desktop are installed.
2. You'll need these directories as they are mounted by the devcontainer:

    - ${localEnv:HOME}/.ssh
    - ${localEnv:HOME}/.config/gcloud

        NOTE: You can just create an empty ~/.config/gcloud, as it's only required by some of the deploy scripts

3. Also set these  up in your .bashrc and make sure they are loaded before invoking vscode:

    - export VSCODE_CONTAINER_GIT_USER="nick.codignotto@gmail.com"
    - export VSCODE_CONTAINER_GIT_EMAIL="Nick Codignotto"

## Projects

### pmud

See [README.md](pmud/README.md) in pmud folder.

### life

```bash
bazel run //life:main -- -c 100 -r 50 -g 10000 -d 100
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
