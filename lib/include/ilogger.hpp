#pragma once

#include <string>

/**
 * Logging severity levels.
 */
enum class LogLevel {
    DEBUG, ///< Dev debug information
    INFO,  ///< General informational messages
    WARN,  ///< Warnings that indicate potential issues
    ERR    ///< Errors that require attention (usually fatal)
};

/**
 * ILogger is an interface for logging messages.
 * Implement this interface to provide custom logging behavior.
 */
class ILogger {
  public:
    virtual ~ILogger()                                           = default;
    virtual void log(LogLevel level, const std::string& message) = 0;
    virtual void setMinimalLogLevel(LogLevel level)              = 0;
};

struct NullLogger : ILogger {
    void log(LogLevel, const std::string&) override {}
    void setMinimalLogLevel(LogLevel level) override {}
};