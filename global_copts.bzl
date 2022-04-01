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

def global_cppopts(unwanted_options = []):
    opts = [
        "-std=c++20",
        "-Wall",
        "-Werror",
        "-Wextra",
        "-Wno-ignored-qualifiers",
        "-Woverloaded-virtual",
        "-Wvla",
        "-Wnoc++17-extensions",
        "-pedantic",
        "-Og",
        "-g",
        "-DSPDLOG_FMT_EXTERNAL",
    ]
    unwanted = {x: x for x in unwanted_options}
    return [opt for opt in opts if unwanted.get(opt) == None]

def global_deps(addl = []):
    return addl + [
        "@fmt//:main",
        "@ncurses//:main",
    ]
