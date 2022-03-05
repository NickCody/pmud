load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "main",
    srcs = [
        "lib/x86_64-linux-gnu/liblua5.4.a",
        "lib/x86_64-linux-gnu/liblua5.4.so",
        "lib/x86_64-linux-gnu/liblua5.4.so.0",
        "lib/x86_64-linux-gnu/liblua5.4.so.0.0.0",
        "lib/x86_64-linux-gnu/liblua5.4-c++.a",
        "lib/x86_64-linux-gnu/liblua5.4-c++.so",
        "lib/x86_64-linux-gnu/liblua5.4-c++.so.0",
        "lib/x86_64-linux-gnu/liblua5.4-c++.so.0.0.0",
    ],
    hdrs = [
        "include/lua5.4/lauxlib.h",
        "include/lua5.4/lua.h",
        "include/lua5.4/lua.hpp",
        "include/lua5.4/luaconf.h",
        "include/lua5.4/lualib.h",
    ],
    strip_include_prefix = "include/lua5.4",
    visibility = ["//visibility:public"],
)
