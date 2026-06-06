#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <string>
#include <memory>

class Logger {
public:
    static void init(const std::string& level = "info") {
        sLogger = spdlog::stdout_color_mt("smallchat");
        sLogger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%s:%#] %v");
        setLevel(level);
    }

    static void setLevel(const std::string& level) {
        if (level == "debug" || level == "DEBUG")
            sLogger->set_level(spdlog::level::debug);
        else if (level == "info" || level == "INFO")
            sLogger->set_level(spdlog::level::info);
        else if (level == "warn" || level == "WARN")
            sLogger->set_level(spdlog::level::warn);
        else if (level == "error" || level == "ERROR")
            sLogger->set_level(spdlog::level::err);
        else
            sLogger->set_level(spdlog::level::info);
    }

    static std::shared_ptr<spdlog::logger> get() {
        return sLogger;
    }

private:
    static std::shared_ptr<spdlog::logger> sLogger;
};

std::shared_ptr<spdlog::logger> Logger::sLogger = nullptr;

#define LOG_DEBUG(...)  if(Logger::get()) Logger::get()->debug(__VA_ARGS__)
#define LOG_INFO(...)   if(Logger::get()) Logger::get()->info(__VA_ARGS__)
#define LOG_WARN(...)   if(Logger::get()) Logger::get()->warn(__VA_ARGS__)
#define LOG_ERROR(...)  if(Logger::get()) Logger::get()->error(__VA_ARGS__)

#endif
