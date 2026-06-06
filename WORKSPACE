load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "spdlog",
    build_file_content = """
cc_library(
    name = "spdlog",
    hdrs = glob([
        "include/**/*.h",
    ]),
    includes = ["include"],
    visibility = ["//visibility:public"],
)
""",
    sha256 = "15cb09808b0c0ac18c0e0e2c85b1b1b6f3c3cb7c9b99c1e2b2d4b2b8b5a3b0b3",
    strip_prefix = "spdlog-1.12.0",
    url = "https://github.com/gabime/spdlog/archive/refs/tags/v1.12.0.tar.gz",
)
