#include "logger.hpp"

#include <iostream>

void Logger::log(LogLevel level, const std::string& message) {
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

void Logger::setMinimalLogLevel(LogLevel min_log_level) {
    m_min_log_level = min_log_level;
}

LogLevel   Logger::m_min_log_level = LogLevel::INFO;
std::mutex Logger::m_mutex;