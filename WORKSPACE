load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")

git_repository(
    name = "com_google_googletest",
    remote = "https://github.com/google/googletest",
    tag = "release-1.11.0",
)

new_local_repository(
    name = "ncurses",
    build_file = "ncurses.BUILD",
    path = "/usr/",
)

new_local_repository(
    name = "hiredis",
    build_file = "hiredis.BUILD",
    path = "/usr/",
)

new_git_repository(
    name = "fmt",
    build_file = "//:fmt.BUILD",
    commit = "6884aab49b1b7fc6dcba1e27999f1aced0b888be",
    remote = "https://github.com/fmtlib/fmt",
    shallow_since = "1641501513 -0800",
)

new_local_repository(
    name = "caf",
    build_file = "caf.BUILD",
    path = "/usr/local",
)

new_local_repository(
    name = "yaml-cpp",
    build_file = "yaml-cpp.BUILD",
    path = "/usr/local",
)

new_local_repository(
    name = "lua",
    build_file = "lua.BUILD",
    path = "/usr",
)

new_local_repository(
    name = "zeromq",
    build_file = "zeromq.BUILD",
    path = "/usr",
)

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Hedron's Compile Commands Extractor for Bazel
# https://github.com/hedronvision/bazel-compile-commands-extractor
http_archive(
    name = "hedron_compile_commands",
    strip_prefix = "bazel-compile-commands-extractor-fa7e9fc11ec4a4de7ea63c6a060108ac612fec0e",

    # Replace the commit hash in both places (below) with the latest, rather than using the stale one here.
    # Even better, set up Renovate and let it do the work for you (see "Suggestion: Updates" in the README).
    url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/fa7e9fc11ec4a4de7ea63c6a060108ac612fec0e.tar.gz",
    # When you first run this tool, it'll recommend a sha256 hash to put here with a message like: "DEBUG: Rule 'hedron_compile_commands' indicated that a canonical reproducible form can be obtained by modifying arguments sha256 = ..."
)

load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")

hedron_compile_commands_setup()
