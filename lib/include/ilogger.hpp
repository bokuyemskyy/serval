#pragma once

#include <string>

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERR
};

class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(LogLevel level, const std::string& message) = 0;
};