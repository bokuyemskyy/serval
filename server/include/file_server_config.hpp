#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

struct PathConfig {
    // CORS
    std::optional<std::vector<std::string>> allow_origin;
    std::optional<bool>                     allow_credentials;
    std::optional<std::vector<std::string>> allow_methods;
    std::optional<std::vector<std::string>> allow_headers;

    // Misc
    std::optional<bool>                                         directory_listing;
    std::optional<bool>                                         show_hidden_files;
    std::optional<std::unordered_map<std::string, std::string>> custom_headers;
    std::optional<std::unordered_map<std::string, std::string>> custom_mime_types;
};

struct Rule {
    std::string path;
    PathConfig  config;
};

class FileServerConfig {
  public:
    std::string       root_directory = ".";
    std::vector<Rule> rules;

    FileServerConfig(std::string root_directory = ".", std::vector<Rule> rules = {})
        : root_directory(std::move(root_directory)), rules(std::move(rules)) {}
};
