#pragma once

#include "../../server/include/logger.hpp"

#include <cxxopts.hpp>
#include <string>
#include <unordered_map>

class HttpServerConfig {
  public:
    std::string host           = "127.0.0.1";
    int         port           = 8080;
    std::string root_directory = ".";

    LogLevel logging_level = LogLevel::INFO;

    bool directory_listing = true;
    bool show_hidden_files = false;

    bool        cors_enabled      = false;
    std::string cors_allow_origin = "*";

    std::unordered_map<std::string, std::string> custom_headers{};
    std::unordered_map<std::string, std::string> custom_mime_types{};

    int worker_threads        = 4;
    int max_connections       = 100;
    int connection_timeout_ms = 30000;

    const size_t MAX_HEADER_BYTES = 8192;
    const size_t MAX_BODY_BYTES = 10 * 1024 * 1024;

    const int REQUEST_TIMEOUT_MS = 5000;
    static HttpServerConfig load(int argc, char* argv[]);

  private:
    void applyCliOverrides(const cxxopts::ParseResult& parse_result);
    void loadFromToml(const std::string& config_path);
};