load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "hello-world",
    srcs = ["src/main.cpp"],
    deps = ["@ncurses//:main"],
)


cc_binary(
    name = "life",
    srcs = ["src/life/life.c"],
    deps = ["@ncurses//:main"],
)

