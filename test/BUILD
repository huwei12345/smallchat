load("@rules_cc//cc:defs.bzl", "cc_test")

package(default_visibility = ["//visibility:public"])

cc_test(
    name = "test_protocol",
    srcs = ["test_protocol.cpp"],
    deps = [
        "//:protocol",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "test_server_config",
    srcs = ["test_server_config.cpp"],
    copts = [
        "-Ithird_party/spdlog/include",
        "-Iinclude",
    ],
    deps = [
        "//:server_lib",
        "@googletest//:gtest_main",
    ],
)
