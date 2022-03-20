load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")

git_repository(
    name = "googletest",
    remote = "https://github.com/google/googletest",
    tag = "release-1.11.0",
)

new_local_repository(
    name = "ncurses",
    build_file = "ncurses.BUILD",
    path = "/usr/",
)

new_local_repository(
    name = "hiredis",
    build_file = "hiredis.BUILD",
    path = "/usr/",
)

new_git_repository(
    name = "fmt",
    build_file = "//:fmt.BUILD",
    commit = "6884aab49b1b7fc6dcba1e27999f1aced0b888be",
    remote = "https://github.com/fmtlib/fmt",
    shallow_since = "1641501513 -0800",
)

new_local_repository(
    name = "caf",
    build_file = "caf.BUILD",
    path = "/usr/local",
)

new_local_repository(
    name = "yaml-cpp",
    build_file = "yaml-cpp.BUILD",
    path = "/usr/local",
)

new_local_repository(
    name = "lua",
    build_file = "lua.BUILD",
    path = "/usr",
)

new_local_repository(
    name = "zeromq",
    build_file = "zeromq.BUILD",
    path = "/usr",
)
