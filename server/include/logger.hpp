#pragma once

#include "ilogger.hpp"

#include <ctime>
#include <mutex>
#include <string>

class Logger : public ILogger {
  public:
    void log(LogLevel level, const std::string& message) override;
    void setMinimalLogLevel(LogLevel min_log_level) override;

  private:
    static LogLevel   m_min_log_level;
    static std::mutex m_mutex;
};
