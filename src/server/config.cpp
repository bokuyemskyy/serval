#include "server/config.hpp"

#include "server/log_level.hpp"

#include <cxxopts.hpp>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <toml++/toml.hpp>

Config Config::load(int argc, char* argv[]) {
    cxxopts::Options options("serval", "Static HTTP server");

    // clang-format off
    options.add_options()
    ("host", "Host to bind", cxxopts::value<std::string>())
    ("p,port", "Port to use", cxxopts::value<int>())
    ("d,directory", "Root directory to serve", cxxopts::value<std::string>())
    ("h,help", "Print help message", cxxopts::value<bool>()->default_value("false"))
    ("c,config", "Config file path", cxxopts::value<std::string>())
    ("no-index", "Disable directory listing", cxxopts::value<bool>()->default_value("false"))
    ("cors", "Enable cors headers", cxxopts::value<bool>()->default_value("false"));
    // clang-format on

    auto parse_result = options.parse(argc, argv);

    if (parse_result.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    std::string config_path;
    if (parse_result.count("config"))
        config_path = parse_result["config"].as<std::string>();

    Config new_config;

    new_config.loadFromToml(config_path);
    new_config.applyCliOverrides(parse_result);

    return new_config;
}

void Config::loadFromToml(const std::string& config_path) {
    std::filesystem::path p(config_path);

    if (!std::filesystem::is_regular_file(p))
        throw std::runtime_error("Config file not found: \'" + config_path + "\'");

    toml::table table;

    table = toml::parse_file(config_path);

    if (auto val = table["host"].value<std::string>()) {
        host = *val;
    }
    if (auto val = table["port"].value<int>()) {
        port = *val;
    }
    if (auto val = table["root_dir"].value<std::string>()) {
        root_directory = *val;
    }
    if (auto val = table["logging"]["enabled"].value<bool>()) {
        logging_enabled = *val;
    }
    if (auto val = table["logging"]["level"].value<std::string>()) {
        if (*val == "debug")
            logging_level = LogLevel::DEBUG;
        else if (*val == "info")
            logging_level = LogLevel::INFO;
        else if (*val == "warn")
            logging_level = LogLevel::WARN;
        else if (*val == "error")
            logging_level = LogLevel::ERR;
    }
    if (auto val = table["features"]["directory_listing"].value<bool>()) {
        directory_listing = *val;
    }
    if (auto val = table["features"]["show_hidden_files"].value<bool>()) {
        show_hidden_files = *val;
    }
    if (auto val = table["cors"]["enabled"].value<bool>()) {
        cors_enable = *val;
    }
    if (auto val = table["cors"]["allow_origin"].value<std::string>()) {
        cors_allow_origin = *val;
    }
    if (auto* headers = table["headers"].as_table()) {
        headers->for_each([&](const toml::key& key, toml::node& value) {
            if (auto str = value.value<std::string>())
                custom_headers[std::string{key.str()}] = *str;
        });
    }
    if (auto* mime_types = table["mime_types"].as_table()) {
        mime_types->for_each([&](const toml::key& key, toml::node& value) {
            if (auto str = value.value<std::string>())
                custom_mime_types[std::string{key.str()}] = *str;
        });
    }
}
void Config::applyCliOverrides(const cxxopts::ParseResult& parse_result) {
    if (parse_result.count("host"))
        host = parse_result["host"].as<std::string>();

    if (parse_result.count("port"))
        port = parse_result["host"].as<int>();

    if (parse_result.count("directory"))
        root_directory = parse_result["directory"].as<std::string>();

    if (parse_result.count("no-index"))
        directory_listing = !parse_result["no-index"].as<bool>();

    if (parse_result.count("cors"))
        cors_enable = parse_result["cors"].as<bool>();
}