load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "binary",
    srcs = ["life.c"],
    deps = ["@ncurses//:main"],
    copts = ["-O3"],
)