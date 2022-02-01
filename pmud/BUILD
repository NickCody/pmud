load("@rules_cc//cc:defs.bzl", "cc_binary")
load("@//:global_copts.bzl", "global_cppopts")
load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")

package(features = ["-default_compile_flags"])

filegroup(
    name = "conf",
    srcs = glob(["conf/*"]),
    
)

filegroup(
    name = "scripts",
    srcs = glob(["scripts/*"]),
)

filegroup(
    name = "caf_so",
    data = ["@caf//:main"],
)

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

pkg_tar(
    name = "dist",
    srcs = [
        "//pmud",
        "//pmud:caf_so",
        "//pmud:conf",
        "//pmud:scripts",
    ],
    extension = "tar.gz",
    include_runfiles = True,
    mode = "0755",
    package_dir = "pmud",
)
