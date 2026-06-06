#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <string>
#include <memory>

class Logger {
public:
    static void init(const std::string& level = "info") {
        auto& logger = getMutable();
        logger = spdlog::stdout_color_mt("smallchat");
        logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%s:%#] %v");
        setLevel(level);
    }

    static void setLevel(const std::string& level) {
        auto& logger = getMutable();
        if (!logger) return;
        if (level == "debug" || level == "DEBUG")
            logger->set_level(spdlog::level::debug);
        else if (level == "info" || level == "INFO")
            logger->set_level(spdlog::level::info);
        else if (level == "warn" || level == "WARN")
            logger->set_level(spdlog::level::warn);
        else if (level == "error" || level == "ERROR")
            logger->set_level(spdlog::level::err);
        else
            logger->set_level(spdlog::level::info);
    }

    static std::shared_ptr<spdlog::logger> get() {
        return getMutable();
    }

private:
    static std::shared_ptr<spdlog::logger>& getMutable() {
        static std::shared_ptr<spdlog::logger> sLogger = nullptr;
        return sLogger;
    }
};

#define LOG_DEBUG(...)  if(Logger::get()) Logger::get()->debug(__VA_ARGS__)
#define LOG_INFO(...)   if(Logger::get()) Logger::get()->info(__VA_ARGS__)
#define LOG_WARN(...)   if(Logger::get()) Logger::get()->warn(__VA_ARGS__)
#define LOG_ERROR(...)  if(Logger::get()) Logger::get()->error(__VA_ARGS__)

#endif
