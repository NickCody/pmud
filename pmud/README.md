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


### Redis

You need a redis instance running and you need to set these variables in your environment:

- `REDIS_HOST`
- `REDIS_PORT`

## nickcoding.com blog

I'm blogging about my adventures here:

- [Building a MUD - Part I](https://www.nickcoding.com/2022/02/18/building-a-mud-part-1/)