#pragma once

#include "cxxopts.hpp"
#include "server/log_level.hpp"

#include <string>
#include <unordered_map>

class Config {
  public:
    std::string host           = "127.0.0.1";
    int         port           = 8080;
    std::string root_directory = ".";

    bool     logging_enabled = true;
    LogLevel logging_level   = LogLevel::INFO;

    bool directory_listing = true;
    bool show_hidden_files = false;

    bool        cors_enable       = false;
    std::string cors_allow_origin = "*";

    std::unordered_map<std::string, std::string> custom_headers{};
    std::unordered_map<std::string, std::string> custom_mime_types{};

    static Config load(int argc, char* argv[]);

  private:
    void applyCliOverrides(const cxxopts::ParseResult& parse_result);
    void loadFromToml(const std::string& config_path);
};