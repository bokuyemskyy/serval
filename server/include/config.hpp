#pragma once

#include "file_server_config.hpp"
#include "http_server_config.hpp"
#include "ilogger.hpp"

#include <cxxopts.hpp>
#include <string>
#include <unordered_map>

class ServerConfig {
  public:
    std::string host           = "127.0.0.1";
    int         port           = 8080;
    std::string root_directory = ".";

    LogLevel logging_level = LogLevel::INFO;

    int worker_threads        = 4;
    int max_connections       = 100;
    int connection_timeout_ms = 30000;

    size_t max_header_bytes = 8192;
    size_t max_body_bytes   = 10 * 1024 * 1024;

    int request_timeout_ms = 5000;

    std::string config_file_used = "";

    std::vector<Rule> rules;

    static ServerConfig load(int argc, char* argv[]);
    FileServerConfig    createFileServerConfig() const;
    HttpServerConfig    createHttpServerConfig() const;

  private:
    void applyCliOverrides(const cxxopts::ParseResult& parse_result);
    void loadFromYaml(const std::string& config_path);
};