#pragma once

#include <ctime>
#include <iostream>
#include <mutex>
#include <string>

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERR
};

class Logger {
  public:
    static void log(LogLevel level, const std::string& message) {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (level < m_min_log_level)
            return;

        std::time_t t           = std::time(nullptr);
        std::string time_string = std::ctime(&t);

        if (time_string.ends_with("\n")) {
            time_string.pop_back();
        }

        std::cout << "[" << time_string << "] ";

        switch (level) {
            case LogLevel::DEBUG:
                std::cout << "[DEBUG] ";
                break;
            case LogLevel::INFO:
                std::cout << "[INFO] ";
                break;
            case LogLevel::WARN:
                std::cerr << "[WARN] ";
                break;
            case LogLevel::ERR:
                std::cerr << "[ERROR] ";
                break;
        }
        std::cout << message << "\n";
    }

    static void setMinLogLevel(LogLevel min_log_level) { m_min_log_level = min_log_level; }

  private:
    static LogLevel   m_min_log_level;
    static std::mutex m_mutex;
};
