load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

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
    commit = "6884aab49b1b7fc6dcba1e27999f1aced0b888be",
    shallow_since = "1641501513 -0800",
    build_file = "//:fmt.BUILD",
)

# new_git_repository(
#     name = "org_actor_framework_caf",
#     remote = "https://github.com/actor-framework/actor-framework",
#     commit = "c2be26e7f9e3e84ef14730590fc4e7b25fb9d29a", 
#     shallow_since = "1626427996 +0200",
#     build_file = "//:caf.BUILD",
# )

http_archive(
    name = "org_actor_framework_caf",
    build_file = "//:caf.BUILD",
    url = "https://github.com/actor-framework/actor-framework/archive/c2be26e7f9e3e84ef14730590fc4e7b25fb9d29a.zip",
)

git_repository(
    name = "boringssl",
    remote = "https://boringssl.googlesource.com/boringssl",
    commit = "5416e4f16bffdc24e71e84d6e3977eed73c6f6b3",
    shallow_since = "1641415239 +0000"
)
