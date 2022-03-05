# Primordia MUD

Welcome to the Primordia MUD

This repo contains the Primordia MUD source code packaged in a devcontainer that has all of the necessary tools and facilities to compile, build, and deploy Primordia MUD.

## Prerequisites / Setup

1. All you need are vcscode and docker desktop for your platform.

2. You'll need these directories as they are mounted by the devcontainer:

    - ${localEnv:HOME}/.ssh
    - ${localEnv:HOME}/.config/gcloud

        NOTE: You can just create an empty ~/.config/gcloud, as it's only required by some of the deploy scripts

3. Also set these  up in your .bashrc and make sure they are loaded before invoking vscode:

    - export VSCODE_CONTAINER_GIT_USER="nick.codignotto@gmail.com"
    - export VSCODE_CONTAINER_GIT_EMAIL="Nick Codignotto"

## > pmud

This is the Primordia MUD, a multiplayer text-based adventure game.

See [README.md](pmud/README.md) in pmud folder.

## > life

```bash
bazel run //life:main -- -c 100 -r 50 -g 10000 -d 100
```

## > graph

```bash
bazel run //graph/application:fully-connected -- 10           # complete graph (dot output)
bazel run //graph/application:random-connected --  10 100     # randomly connected (dot output)
bazel run //graph/application:matrix -- 10 100                # prints adjacency matrix representation (custom output)
bazel run //graph/application:structure --  10 100            # prints adj-list structure (custom output)
bazel run //graph/application:bfs-search --  10 100           # Does a BFS from vertex 1 -> n
```

## > util

Multiple small utilities and test programs.

```
bazel run //util:cppversion           # shows which C++ version is configured
bazel run //util:lua                  # basic lua interpreter
bazel run //util:scratch              # scratch, throwaway code
bazel run //util:yaml-print           # prints a yaml file with some basic type information
bazel run //util:taml-redis-loader    # loads yaml into redis
bazel run //util:
```

## nickcoding.com blog

I'm blogging about my adventures here on my [blog](https://www.nickcoding.com)

Posts:

- [Building a MUD - Part I](https://www.nickcoding.com/2022/02/18/building-a-mud-part-1/)