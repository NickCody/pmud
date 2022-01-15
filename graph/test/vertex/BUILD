load("@rules_cc//cc:defs.bzl", "cc_test")
load("@//:global_copts.bzl", "global_cppopts")

cc_test(
  name = "test",
  srcs = glob(["**/*.cpp"]),
  deps = [
    "//graph:headers",
    "@googletest//:gtest_main",
  ],
  copts = global_cppopts(),
)