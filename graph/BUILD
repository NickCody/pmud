load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "library",
    srcs = ["main.cpp"],
    hdrs = ["graph.h"],
    deps = ["@ncurses//:main"],
)

cc_binary(
    name = "binary",
    deps = ["library", "@ncurses//:main"],
)