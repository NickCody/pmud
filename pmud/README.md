# Primordia MUD

## To Build and Run

Build this way:

```
bazel build //primordia-mud:pmud
```

Run this way:

```
bazel-bin/primordia-mud/pmud --config-file=.caf_pmud 
```

## Redis

You need a redis instance running and you need to set these variables in your environment:

- `REDIS_HOST`
- `REDIS_PORT`

