#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>

#define INFO(message) logger.log(INFO, message)
#define WARNING(message) logger.log(WARNING, message)
#define ERROR(message) logger.log(ERROR, message)

enum LogLevel {
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    Logger() : log_level(INFO), log_file("log.txt") {}

    void setLogLevel(LogLevel level) {
        log_level = level;
    }

    void log(LogLevel level, const std::string &message) {
        if (level >= log_level) {
            std::lock_guard<std::mutex> guard(log_mutex);
            std::string level_str = getLevelString(level);
            std::cout << level_str << ": " << message << std::endl;
            log_file << level_str << ": " << message << std::endl;
        }
    }

private:
    LogLevel log_level;
    std::ofstream log_file;
    std::mutex log_mutex;

    std::string getLevelString(LogLevel level) {
        switch (level) {
            case INFO: return "INFO";
            case WARNING: return "WARNING";
            case ERROR: return "ERROR";
            default: return "UNKNOWN";
        }
    }
};

extern Logger logger;

#endif // LOGGER_H