load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "protocol",
    srcs = ["MyProtocolStream.cpp"],
    hdrs = [
        "MyProtocolStream.h",
        "Protocol.h",
        "soft.h",
    ],
    includes = ["."],
)

cc_library(
    name = "server_lib",
    srcs = [
        "EventLoop.cpp",
        "FileTools.cpp",
        "MysqlPool.cpp",
        "RequestProcessor.cpp",
        "ServerConfig.cpp",
        "cache/friendCache.cpp",
        "server.cpp",
        "settime.cpp",
    ],
    hdrs = [
        "EventLoop.h",
        "FileTools.h",
        "MysqlPool.h",
        "Protocol.h",
        "RequestProcessor.h",
        "ServerConfig.h",
        "Trans.h",
        "cache/friendCache.h",
        "include/Logger.h",
        "mutex.h",
        "server.h",
        "settime.h",
        "soft.h",
        "threadPool.h",
    ],
    copts = [
        "-Ithird_party/spdlog/include",
        "-Iinclude",
    ],
    includes = ["."],
    linkopts = [
        "-lpthread",
        "-lmysqlcppconn",
    ],
    deps = [
        ":protocol",
        "@spdlog",
    ],
)

cc_binary(
    name = "server",
    srcs = ["main.cpp"],
    deps = [
        ":server_lib",
    ],
)
