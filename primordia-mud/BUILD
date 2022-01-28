load("@rules_cc//cc:defs.bzl", "cc_binary")
load("@//:global_copts.bzl", "global_cppopts")

package(features = ["-default_compile_flags"])

cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    copts = global_cppopts(),
    linkopts = [
        "-pthread",
        "-lstdc++fs",
    ],
    deps = [
        "//graph:library",
        "@caf//:main",
        "@fmt//:main",
    ],
)
