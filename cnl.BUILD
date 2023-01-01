load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "main",
    hdrs = glob([
        "include/cnl/**/*.h",
    ]),
    includes = [
        "include",
    ],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
)
