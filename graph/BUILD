load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "library",
    srcs = ["graph.cpp"],
    deps = ["@ncurses//:main"],
)

cc_binary(
    name = "binary",
    deps = ["library", "@ncurses//:main"],
)