load("@rules_cc//cc:defs.bzl", "cc_binary")
load("@//:global_copts.bzl", "global_cppopts")

cc_binary(
    name = "random-connected",
    srcs = ["random-connected.cpp"],
    copts = global_cppopts(),
    deps = ["//pmud/graph:library"],
)

cc_binary(
    name = "fully-connected",
    srcs = ["fully-connected.cpp"],
    copts = global_cppopts(),
    deps = ["//pmud/graph:library"],
)

cc_binary(
    name = "structure",
    srcs = ["structure.cpp"],
    copts = global_cppopts(),
    deps = ["//pmud/graph:library"],
)

cc_binary(
    name = "matrix",
    srcs = ["matrix.cpp"],
    copts = global_cppopts(),
    deps = ["//pmud/graph:library"],
)

cc_binary(
    name = "transpose",
    srcs = ["transpose.cpp"],
    copts = global_cppopts(),
    deps = ["//pmud/graph:library"],
)

cc_binary(
    name = "bfs-search",
    srcs = ["bfs-search.cpp"],
    copts = global_cppopts(),
    deps = ["//pmud/graph:library"],
)

cc_binary(
    name = "dfs-search",
    srcs = ["dfs-search.cpp"],
    copts = global_cppopts(),
    deps = ["//pmud/graph:library"],
)
