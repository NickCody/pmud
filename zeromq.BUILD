load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "main",
    srcs = [
        "lib/x86_64-linux-gnu/libz3.so",
        "lib/x86_64-linux-gnu/libz3.so.4",
    ],
    hdrs = [
        "include/zmq.h",
        "include/zmq.hpp",
        "include/zmq_addon.hpp",
        "include/zmq_utils.h",
    ],
    strip_include_prefix = "include/",
    visibility = ["//visibility:public"],
)
