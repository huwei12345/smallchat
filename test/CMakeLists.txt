cmake_minimum_required(VERSION 3.14)
project(smallchat_tests)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 源文件路径
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

# 测试可执行文件 - Protocol 序列化测试
add_executable(test_protocol
    test_protocol.cpp
    ${ROOT_DIR}/MyProtocolStream.cpp
)
target_include_directories(test_protocol PRIVATE ${ROOT_DIR})
target_compile_definitions(test_protocol PRIVATE SERVER)
target_link_libraries(test_protocol gtest_main)

# 测试可执行文件 - ServerConfig 测试
add_executable(test_server_config
    test_server_config.cpp
    ${ROOT_DIR}/ServerConfig.cpp
)
target_include_directories(test_server_config PRIVATE ${ROOT_DIR})
target_compile_definitions(test_server_config PRIVATE SERVER)
target_link_libraries(test_server_config gtest_main)

include(GoogleTest)
gtest_discover_tests(test_protocol)
gtest_discover_tests(test_server_config)
