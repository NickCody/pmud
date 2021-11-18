load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "graph",
    hdrs = ["graph.h"],
    deps = ["@ncurses//:main"],
)

cc_binary(
    name = "random-connected",
    srcs = ["random-connected.cpp"],
    deps = ["graph"],
)
cc_binary(
    name = "fully-connected",
    srcs = ["fully-connected.cpp"],
    deps = ["graph"],
)