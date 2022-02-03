# Primordia MUD

## Introduction

This is the Primordia MUD, a multiplayer text-based adventure game.

### To Build and Run

Build this way:

```
bazel build //primordia-mud:pmud
```

Run this way:

```
bazel-bin/primordia-mud/pmud --config-file=.caf_pmud 
```


## Feature Roadmap

1. Create abstraction around I/O
    - Move actors into classes (solve the initialization problem)
    - Create User class that persists user information
        - Make storage abstraction
1. User Information
    -  Store ip and time of login
    -  Login, create username/password (write to data directory) 
    -  Commands to print whoami, connection info, connection history
1. ncurses
    - Experiment with ncurses to see what's capable
    - Experiment with color
    