#pragma once

#include "ilogger.hpp"

#include <cxxopts.hpp>
#include <string>
#include <unordered_map>

class HttpServerConfig {
  public:
    std::string host = "127.0.0.1";
    int         port = 8080;

    LogLevel logging_level = LogLevel::INFO;

    bool        cors_enabled      = false;
    std::string cors_allow_origin = "*";

    std::unordered_map<std::string, std::string> custom_headers{};

    int worker_threads        = 4;
    int max_connections       = 100;
    int connection_timeout_ms = 30000;

    size_t max_header_bytes = 8192;
    size_t max_body_bytes   = 10 * 1024 * 1024;

    int request_timeout_ms = 5000;

    class Builder;
};

class HttpServerConfig::Builder {
  public:
    Builder& setHost(const std::string& h) {
        m_config.host = h;
        return *this;
    }

    Builder& setPort(int p) {
        m_config.port = p;
        return *this;
    }
    Builder& setLogger(std::shared_ptr<ILogger> l) {
        m_config.logger = std::move(l);
        return *this;
    }

    Builder& setLoggingLevel(LogLevel level) {
        m_config.logging_level = level;
        return *this;
    }

    Builder& enableCors(bool enabled = true) {
        m_config.cors_enabled = enabled;
        return *this;
    }

    Builder& setCorsAllowOrigin(const std::string& origin) {
        m_config.cors_allow_origin = origin;
        return *this;
    }

    Builder& addCustomHeader(const std::string& key, const std::string& value) {
        m_config.custom_headers[key] = value;
        return *this;
    }

    Builder& setCustomHeaders(const std::unordered_map<std::string, std::string>& headers) {
        m_config.custom_headers = headers;
        return *this;
    }

    Builder& setWorkerThreads(int threads) {
        m_config.worker_threads = threads;
        return *this;
    }

    Builder& setMaxConnections(int max) {
        m_config.max_connections = max;
        return *this;
    }

    Builder& setConnectionTimeoutMs(int timeout) {
        m_config.connection_timeout_ms = timeout;
        return *this;
    }

    Builder& setMaxHeaderBytes(size_t bytes) {
        m_config.max_header_bytes = bytes;
        return *this;
    }

    Builder& setMaxBodyBytes(size_t bytes) {
        m_config.max_body_bytes = bytes;
        return *this;
    }

    Builder& setRequestTimeoutMs(int timeout) {
        m_config.request_timeout_ms = timeout;
        return *this;
    }

    HttpServerConfig build() { return m_config; }

  private:
    HttpServerConfig m_config;
};
