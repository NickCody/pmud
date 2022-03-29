load("@rules_cc//cc:defs.bzl", "cc_test")
load("@//:global_copts.bzl", "global_cppopts")

cc_test(
    name = "test",
    srcs = glob(["**/*.cpp"]),
    copts = global_cppopts(),
    deps = [
        "//pmud/graph:library",
        "@com_google_googletest//:gtest_main",
    ],
)
