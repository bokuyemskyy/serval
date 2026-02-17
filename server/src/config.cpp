#include "config.hpp"

#include <cxxopts.hpp>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <toml++/toml.hpp>

ServerConfig ServerConfig::load(int argc, char* argv[]) {
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

    ServerConfig new_config;

    std::string config_path;
    if (parse_result.count("config")) {
        config_path = parse_result["config"].as<std::string>();
    } else {
        for (const auto& name : {"server.toml", "config.toml", ".serverrc"}) {
            if (std::filesystem::exists(name)) {
                config_path = name;
                break;
            }
        }
    }

    if (!config_path.empty()) {
        try {
            new_config.loadFromToml(config_path);
        } catch (const std::exception& e) {
            throw std::runtime_error("Error loading config file: " + std::string(e.what()));
        }
    }

    new_config.applyCliOverrides(parse_result);

    return new_config;
}
FileServerConfig ServerConfig::createFileServerConfig() const {
    return FileServerConfig::Builder()
        .setRootDirectory(root_directory)
        .enableDirectoryListing(directory_listing)
        .enableShowHiddenFiles(show_hidden_files)
        .setCustomMimeTypes(custom_mime_types)
        .build();
}
HttpServerConfig ServerConfig::createHttpServerConfig() const {
    return HttpServerConfig::Builder()
        .setHost(host)
        .setPort(port)
        .setLoggingLevel(logging_level)
        .enableCors(cors_enabled)
        .setCorsAllowOrigin(cors_allow_origin)
        .setCustomHeaders(custom_headers)
        .setWorkerThreads(worker_threads)
        .setMaxConnections(max_connections)
        .setConnectionTimeoutMs(connection_timeout_ms)
        .build();
}

void ServerConfig::loadFromToml(const std::string& config_path) {
    std::filesystem::path p(config_path);

    if (!std::filesystem::is_regular_file(p))
        throw std::runtime_error("Config file not found: \'" + config_path + "\'");

    toml::table toml = toml::parse_file(config_path);

    host           = toml["host"].value_or(host);
    port           = toml["port"].value_or(port);
    root_directory = toml["root_dir"].value_or(root_directory);

    if (auto logging = toml["logging"].as_table()) {
        std::string logging_level_str = logging->get("level")->value_or("info");

        if (logging_level_str == "debug")
            logging_level = LogLevel::DEBUG;
        else if (logging_level_str == "info")
            logging_level = LogLevel::INFO;
        else if (logging_level_str == "warn")
            logging_level = LogLevel::WARN;
        else if (logging_level_str == "error")
            logging_level = LogLevel::ERR;
    }

    if (auto features = toml["features"].as_table()) {
        directory_listing = features->get("directory_listing")->value_or(directory_listing);
        show_hidden_files = features->get("show_hidden_files")->value_or(show_hidden_files);
    }

    if (auto cors = toml["cors"].as_table()) {
        cors_enabled      = cors->get("enabled")->value_or(cors_enabled);
        cors_allow_origin = cors->get("allow_origin")->value_or(cors_allow_origin);
    }

    if (auto threading = toml["threading"].as_table()) {
        worker_threads        = threading->get("worker_threads")->value_or(worker_threads);
        max_connections       = threading->get("max_connections")->value_or(max_connections);
        connection_timeout_ms = threading->get("connection_timeout_ms")->value_or(connection_timeout_ms);
    }

    if (auto headers = toml["headers"].as_table()) {
        headers->for_each([this](const toml::key& key, auto&& val) {
            if (val.is_string()) {
                custom_headers[std::string(key)] = val.as_string()->get();
            }
        });
    }

    if (auto mime = toml["mime_types"].as_table()) {
        mime->for_each([this](const toml::key& key, auto&& val) {
            if (val.is_string()) {
                custom_mime_types[std::string(key)] = val.as_string()->get();
            }
        });
    }

    config_file_used = config_path;
}

void ServerConfig::applyCliOverrides(const cxxopts::ParseResult& parse_result) {
    if (parse_result.count("host"))
        host = parse_result["host"].as<std::string>();

    if (parse_result.count("port"))
        port = parse_result["host"].as<int>();

    if (parse_result.count("directory"))
        root_directory = parse_result["directory"].as<std::string>();

    if (parse_result.count("no-index"))
        directory_listing = !parse_result["no-index"].as<bool>();

    if (parse_result.count("cors"))
        cors_enabled = parse_result["cors"].as<bool>();
}