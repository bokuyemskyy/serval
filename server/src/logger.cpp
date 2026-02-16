#include "logger.hpp"

LogLevel   Logger::m_min_log_level = LogLevel::INFO;
std::mutex Logger::m_mutex;