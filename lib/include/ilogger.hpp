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
    virtual void setMinimalLogLevel(LogLevel level) = 0;
};

struct NullLogger : ILogger {
    void log(LogLevel, const std::string&) override {}
    void setMinimalLogLevel(LogLevel level) override {}
};