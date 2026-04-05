#include "config.hpp"

#include "file_server_config.hpp"

#include <cxxopts.hpp>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <yaml-cpp/yaml.h>

ServerConfig ServerConfig::load(int argc, char* argv[]) {
    cxxopts::Options options("serval", "Static HTTP server");

    // clang-format off
    options.add_options()
    ("host", "Host to bind", cxxopts::value<std::string>())
    ("p,port", "Port to use", cxxopts::value<int>())
    ("d,directory", "Root directory to serve", cxxopts::value<std::string>())
    ("h,help", "Print help message", cxxopts::value<bool>()->default_value("false"))
    ("c,config", "Config file path", cxxopts::value<std::string>());
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
        for (const auto& name : {"server.yaml", "config.yaml", "server.yml", "config.yml", ".serverrc"}) {
            if (std::filesystem::exists(name)) {
                config_path = name;
                break;
            }
        }
    }

    if (!config_path.empty()) {
        try {
            new_config.loadFromYaml(config_path);
        } catch (const std::exception& e) {
            throw std::runtime_error("Error loading config file: " + std::string(e.what()));
        }
    }

    new_config.applyCliOverrides(parse_result);

    return new_config;
}
FileServerConfig ServerConfig::createFileServerConfig() const {
    return FileServerConfig(root_directory, rules);
}
HttpServerConfig ServerConfig::createHttpServerConfig() const {
    return HttpServerConfig::Builder()
        .setHost(host)
        .setPort(port)
        .setLoggingLevel(logging_level)
        .setWorkerThreads(worker_threads)
        .setMaxConnections(max_connections)
        .setConnectionTimeoutMs(connection_timeout_ms)
        .build();
}

void ServerConfig::loadFromYaml(const std::string& config_path) {
    std::filesystem::path file_path(config_path);

    if (!std::filesystem::is_regular_file(file_path))
        throw std::runtime_error("Config file not found: \'" + config_path + "\'");

    YAML::Node yaml = YAML::LoadFile(config_path);

    if (yaml["host"] && yaml["host"].IsScalar())
        host = yaml["host"].as<std::string>();
    port           = yaml["port"] ? yaml["port"].as<int>() : port;
    root_directory = yaml["root_directory"] ? yaml["root_directory"].as<std::string>() : root_directory;

    if (yaml["logging"]) {
        auto log = yaml["logging"];

        if (log.IsScalar()) {
            std::string s = log.as<std::string>();

            if (s == "debug")
                logging_level = LogLevel::DEBUG;
            else if (s == "info")
                logging_level = LogLevel::INFO;
            else if (s == "warn")
                logging_level = LogLevel::WARN;
            else if (s == "error")
                logging_level = LogLevel::ERR;
        }
    }
    if (yaml["threading"]) {
        auto threading = yaml["threading"];

        worker_threads  = threading["worker_threads"] ? threading["worker_threads"].as<int>() : worker_threads;
        max_connections = threading["max_connections"] ? threading["max_connections"].as<int>() : max_connections;
        connection_timeout_ms =
            threading["connection_timeout_ms"] ? threading["connection_timeout_ms"].as<int>() : connection_timeout_ms;
    }

    if (yaml["rules"]) {
        auto rule_nodes = yaml["rules"];

        for (const auto& rule_node : rule_nodes) {
            if (rule_node["path"] && rule_node["config"]) {
                PathConfig config{};

                auto cfg = rule_node["config"];

                std::string path = rule_node["path"].as<std::string>();

                if (cfg["directory_listing"])
                    config.directory_listing = cfg["directory_listing"].as<bool>();
                if (cfg["show_hidden_files"])
                    config.show_hidden_files = cfg["show_hidden_files"].as<bool>();
                if (cfg["cors"]) {
                    auto cors = cfg["cors"];

                    if (cors["allow_origin"]) {
                        if (cors["allow_origin"].IsScalar()) {
                            config.allow_origin = std::vector<std::string>{cors["allow_origin"].as<std::string>()};
                        } else {
                            std::vector<std::string> origins;
                            for (const auto& o : cors["allow_origin"])
                                origins.push_back(o.as<std::string>());
                            config.allow_origin = origins;
                        }
                    }

                    if (cors["allow_credentials"])
                        config.allow_credentials = cors["allow_credentials"].as<bool>();

                    if (cors["allow_methods"]) {
                        std::vector<std::string> methods;
                        for (const auto& m : cors["allow_methods"])
                            methods.push_back(m.as<std::string>());
                        config.allow_methods = methods;
                    }

                    if (cors["allow_headers"]) {
                        std::vector<std::string> headers;
                        for (const auto& h : cors["allow_headers"])
                            headers.push_back(h.as<std::string>());
                        config.allow_headers = headers;
                    }

                    if (cors["custom_headers"]) {
                        std::unordered_map<std::string, std::string> headers;

                        for (const auto& h : cors["custom_headers"]) {
                            headers[h.first.as<std::string>()] = h.second.as<std::string>();
                        }

                        config.custom_headers = headers;
                    }
                    if (cors["custom_mime_types"]) {
                        std::unordered_map<std::string, std::string> mime;

                        for (const auto& m : cors["custom_mime_types"]) {
                            mime[m.first.as<std::string>()] = m.second.as<std::string>();
                        }

                        config.custom_mime_types = mime;
                    }
                }

                rules.emplace_back(Rule{rule_node["path"].as<std::string>(), config});
            }
        }
    }

    config_file_used = config_path;
}

void ServerConfig::applyCliOverrides(const cxxopts::ParseResult& parse_result) {
    if (parse_result.count("host"))
        host = parse_result["host"].as<std::string>();

    if (parse_result.count("port"))
        port = parse_result["port"].as<int>();

    if (parse_result.count("directory"))
        root_directory = parse_result["directory"].as<std::string>();
}