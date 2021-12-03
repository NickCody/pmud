load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")

git_repository(
    name = "googletest",
    remote = "https://github.com/google/googletest",
    tag = "release-1.11.0",
)

new_local_repository(
    name = "ncurses",
    path = "/usr/", # Use this for Ubuntu
    build_file = "ncurses.BUILD",
)

new_git_repository(
    name = "fmt",
    remote = "https://github.com/fmtlib/fmt",
    branch = "master",
    build_file = "//:fmt.BUILD",
)