load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "googletest",
    remote = "https://github.com/google/googletest",
    tag = "release-1.8.1",
)

new_local_repository(
    name = "ncurses",
    path = "/usr/", # Use this for Ubuntu
    build_file = "ncurses.BUILD",
)