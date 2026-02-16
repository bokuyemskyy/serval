#pragma once

#include "ilogger.hpp"

#include <ctime>
#include <iostream>
#include <mutex>
#include <string>

class Logger : public ILogger {
  public:
    void log(LogLevel level, const std::string& message);

    // void setMinLogLevel(LogLevel min_log_level) { m_min_log_level = min_log_level; }

  private:
    static LogLevel   m_min_log_level;
    static std::mutex m_mutex;
};
