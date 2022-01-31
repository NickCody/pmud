load("@rules_cc//cc:defs.bzl", "cc_binary")
load("@//:global_copts.bzl", "global_cppopts")

package(features = ["-default_compile_flags"])

cc_binary(
    name = "pmud",
    srcs = glob([
        "**/*.cpp",
        "**/*.h",
    ]),
    copts = global_cppopts(),
    linkopts = [
        "-pthread",
        "-lstdc++fs",
    ],
    deps = [
        "//graph:library",
        "@caf//:main",
        "@fmt//:main",
        "@yaml-cpp//:main",
    ],
)
