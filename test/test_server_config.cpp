#include <gtest/gtest.h>
#include "ServerConfig.h"
#include <fstream>
#include <cstdio>

static void writeTempFile(const char* path, const char* content) {
    std::ofstream f(path);
    f << content;
    f.close();
}

TEST(ServerConfig, LoadValidConfig) {
    const char* path = "/tmp/test_config_valid.ini";
    writeTempFile(path,
        "[server]\n"
        "port = 9090\n"
        "io_threads = 4\n"
        "\n"
        "[mysql]\n"
        "host = tcp://192.168.1.1:3306/testdb\n"
        "user = admin\n"
        "password = secret123\n"
    );

    ServerConfig* config = ServerConfig::GetInstance();
    EXPECT_TRUE(config->load(path));

    EXPECT_EQ(9090, config->getInt("server", "port", 8080));
    EXPECT_EQ(4, config->getInt("server", "io_threads", 2));
    EXPECT_EQ("tcp://192.168.1.1:3306/testdb", config->getString("mysql", "host", ""));
    EXPECT_EQ("admin", config->getString("mysql", "user", ""));
    EXPECT_EQ("secret123", config->getString("mysql", "password", ""));

    std::remove(path);
}

TEST(ServerConfig, GetDefaultValues) {
    const char* path = "/tmp/test_config_defaults.ini";
    writeTempFile(path,
        "[server]\n"
        "port = 8080\n"
    );

    ServerConfig* config = ServerConfig::GetInstance();
    config->load(path);

    // 存在的 key
    EXPECT_EQ(8080, config->getInt("server", "port", 0));

    // 不存在的 key，返回默认值
    EXPECT_EQ(2, config->getInt("server", "io_threads", 2));
    EXPECT_EQ("default_host", config->getString("mysql", "host", "default_host"));

    // 不存在的 section
    EXPECT_EQ(99, config->getInt("nonexistent", "key", 99));

    std::remove(path);
}

TEST(ServerConfig, CommentLinesIgnored) {
    const char* path = "/tmp/test_config_comments.ini";
    writeTempFile(path,
        "# This is a comment\n"
        "; This is also a comment\n"
        "[server]\n"
        "# port = 1111\n"
        "port = 8080\n"
        "; io_threads = 99\n"
        "io_threads = 2\n"
    );

    ServerConfig* config = ServerConfig::GetInstance();
    config->load(path);

    EXPECT_EQ(8080, config->getInt("server", "port", 0));
    EXPECT_EQ(2, config->getInt("server", "io_threads", 0));

    std::remove(path);
}

TEST(ServerConfig, WhitespaceTrimmed) {
    const char* path = "/tmp/test_config_spaces.ini";
    writeTempFile(path,
        "[server]\n"
        "  port  =  9090  \n"
        "\tio_threads\t=\t4\t\n"
    );

    ServerConfig* config = ServerConfig::GetInstance();
    config->load(path);

    EXPECT_EQ(9090, config->getInt("server", "port", 0));
    EXPECT_EQ(4, config->getInt("server", "io_threads", 0));

    std::remove(path);
}

TEST(ServerConfig, NonexistentFile) {
    ServerConfig* config = ServerConfig::GetInstance();
    EXPECT_FALSE(config->load("/tmp/nonexistent_config_file_12345.ini"));
}

TEST(ServerConfig, EmptyAndBlankLines) {
    const char* path = "/tmp/test_config_empty.ini";
    writeTempFile(path,
        "\n"
        "   \n"
        "[server]\n"
        "\n"
        "port = 8080\n"
        "   \n"
        "\n"
    );

    ServerConfig* config = ServerConfig::GetInstance();
    config->load(path);

    EXPECT_EQ(8080, config->getInt("server", "port", 0));

    std::remove(path);
}

TEST(ServerConfig, MultipleSections) {
    const char* path = "/tmp/test_config_multi.ini";
    writeTempFile(path,
        "[section_a]\n"
        "key1 = value_a\n"
        "\n"
        "[section_b]\n"
        "key1 = value_b\n"
        "\n"
        "[section_c]\n"
        "key2 = 42\n"
    );

    ServerConfig* config = ServerConfig::GetInstance();
    config->load(path);

    EXPECT_EQ("value_a", config->getString("section_a", "key1", ""));
    EXPECT_EQ("value_b", config->getString("section_b", "key1", ""));
    EXPECT_EQ(42, config->getInt("section_c", "key2", 0));

    std::remove(path);
}

TEST(ServerConfig, IntParseError) {
    const char* path = "/tmp/test_config_badint.ini";
    writeTempFile(path,
        "[server]\n"
        "port = not_a_number\n"
    );

    ServerConfig* config = ServerConfig::GetInstance();
    config->load(path);

    // stoi 抛异常时应返回默认值
    EXPECT_EQ(8080, config->getInt("server", "port", 8080));

    std::remove(path);
}
