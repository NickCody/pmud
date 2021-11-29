load("@rules_cc//cc:defs.bzl", "cc_test")
load("@//:global_copts.bzl", "global_copts")

cc_test(
  name = "tests",
  srcs = glob(["**/*.cpp"]),
  deps = [
    "//graph:headers",
    "@googletest//:gtest_main",
  ],
  copts = global_copts(),
)