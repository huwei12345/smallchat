#include "ServerConfig.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Logger.h"

ServerConfig* ServerConfig::GetInstance()
{
    static ServerConfig instance;
    return &instance;
}

bool ServerConfig::load(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        printf("Warning: Cannot open config file '%s', using defaults\n", filename.c_str());
        LOG_WARN("cannot open config file '{}', using defaults", filename);
        return false;
    }

    std::string line;
    std::string currentSection;

    while (std::getline(file, line)) {
        // 去除首尾空格
        size_t start = line.find_first_not_of(" \t");
        if (start == std::string::npos) continue;
        line = line.substr(start);
        size_t end = line.find_last_not_of(" \t\r\n");
        if (end != std::string::npos) line = line.substr(0, end + 1);

        // 跳过注释和空行
        if (line.empty() || line[0] == '#' || line[0] == ';') continue;

        // 解析 section
        if (line[0] == '[' && line.back() == ']') {
            currentSection = line.substr(1, line.size() - 2);
            continue;
        }

        // 解析 key=value
        size_t eqPos = line.find('=');
        if (eqPos != std::string::npos && !currentSection.empty()) {
            std::string key = line.substr(0, eqPos);
            std::string value = line.substr(eqPos + 1);

            // 去除 key 和 value 的空格
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));

            mConfig[currentSection][key] = value;
        }
    }

    file.close();
    printf("Config loaded from '%s'\n", filename.c_str());
    LOG_INFO("config loaded from '{}'", filename);
    return true;
}

std::string ServerConfig::getString(const std::string& section, const std::string& key, const std::string& defaultValue)
{
    auto secIt = mConfig.find(section);
    if (secIt != mConfig.end()) {
        auto keyIt = secIt->second.find(key);
        if (keyIt != secIt->second.end()) {
            return keyIt->second;
        }
    }
    return defaultValue;
}

int ServerConfig::getInt(const std::string& section, const std::string& key, int defaultValue)
{
    std::string value = getString(section, key, "");
    if (value.empty()) {
        return defaultValue;
    }
    try {
        return std::stoi(value);
    } catch (...) {
        return defaultValue;
    }
}
