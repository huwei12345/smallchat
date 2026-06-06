#ifndef SERVER_CONFIG_H
#define SERVER_CONFIG_H

#include <string>
#include <map>

class ServerConfig {
public:
    static ServerConfig* GetInstance();

    bool load(const std::string& filename = "server_config.ini");

    std::string getString(const std::string& section, const std::string& key, const std::string& defaultValue = "");
    int getInt(const std::string& section, const std::string& key, int defaultValue = 0);

    // 便捷方法
    int getServerPort() { return getInt("server", "port", 8080); }
    int getIOThreads() { return getInt("server", "io_threads", 2); }

    std::string getMysqlHost() { return getString("mysql", "host", "tcp://127.0.0.1:3306/mIM"); }
    std::string getMysqlUser() { return getString("mysql", "user", "root"); }
    std::string getMysqlPassword() { return getString("mysql", "password", "123456"); }
    int getMysqlPoolSize() { return getInt("mysql", "pool_size", 4); }

    int getHeartbeatInterval() { return getInt("heartbeat", "check_interval", 60); }
    int getHeartbeatTimeout() { return getInt("heartbeat", "timeout", 90); }

private:
    ServerConfig() = default;
    std::map<std::string, std::map<std::string, std::string>> mConfig;
};

#endif
