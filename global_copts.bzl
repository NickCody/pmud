"""Global gcc compilere options"""

def global_copts(unwanted_options = []):
    opts = [
        "-Wall",
        "-Werror",
        "-Wextra",
        "-Wno-ignored-qualifiers",
        "-Wvla",
        "-pedantic",
        "-Og",
        "-g",
    ]
    unwanted = {x: x for x in unwanted_options}
    return [opt for opt in opts if unwanted.get(opt) == None]

def global_cpp20(unwanted_options = []):
    opts = [
        "-std=c++20",
    ]
    unwanted = {x: x for x in unwanted_options}
    return [opt for opt in opts if unwanted.get(opt) == None]

def global_cpp17(unwanted_options = []):
    opts = [
        "-std=c++17",
    ]
    unwanted = {x: x for x in unwanted_options}
    return [opt for opt in opts if unwanted.get(opt) == None]

def global_cpp2b(unwanted_options = []):
    opts = [
        "-std=c++2b",
    ]
    unwanted = {x: x for x in unwanted_options}
    return [opt for opt in opts if unwanted.get(opt) == None]

def global_cppopts(unwanted_options = []):
    opts = [
        "-std=c++20",
        "-Wall",
        "-Werror",
        "-Wextra",
        "-Wno-ignored-qualifiers",
        "-Woverloaded-virtual",
        "-Wvla",
        "-Wno-unused-const-variable",
        "-pedantic",
        "-O3",
        # "-g",
        # "-ggdb",
        "-DSPDLOG_FMT_EXTERNAL",
        "-fdiagnostics-color=always",
        "-fdiagnostics-path-format=inline-events",
    ]
    unwanted = {x: x for x in unwanted_options}
    return [opt for opt in opts if unwanted.get(opt) == None]

def global_linkopts(unwanted_options = []):
    opts = [
        "-pthread",
        "-lstdc++fs",
        # "-g",
        "-ldl",
        "-lzmq",
    ]
    unwanted = {x: x for x in unwanted_options}
    return [opt for opt in opts if unwanted.get(opt) == None]

def global_deps(addl = []):
    return addl + [
        "@fmt//:main",
        "@ncurses//:main",
    ]
