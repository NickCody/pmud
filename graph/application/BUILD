load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
load("@//:global_copts.bzl", "global_copts")

cc_library(
    name = "graph",
    hdrs = ["include/graph.h"],
    deps = ["@ncurses//:main"],
    strip_include_prefix = "include",
)

cc_binary(
    name = "random-connected",
    srcs = ["random-connected.cpp"],
    deps = ["graph"],
    copts = global_copts(),
)

cc_binary(
    name = "fully-connected",
    srcs = ["fully-connected.cpp"],
    deps = ["graph"],
    copts = global_copts(),
)

cc_binary(
    name = "structure",
    srcs = ["structure.cpp"],
    deps = ["graph"],
    copts = global_copts(),
)

cc_binary(
    name = "matrix",
    srcs = ["matrix.cpp"],
    deps = ["graph"],
    copts = global_copts(),
)


cc_binary(
    name = "transpose",
    srcs = ["transpose.cpp"],
    deps = ["graph"],
    copts = global_copts(),
)

cc_binary(
    name = "cppversion",
    srcs = ["cppversion.cpp"],
    deps = ["graph"],
    copts = global_copts(),
)