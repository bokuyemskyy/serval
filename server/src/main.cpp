
#include "../../lib/include/http_server.hpp"
#include "../include/config.hpp"
#include "logger.hpp"
#include "static_file_server.hpp"

#include <csignal>
#include <memory>

std::unique_ptr<HttpServer> g_server;
std::shared_ptr<Logger>     g_logger;

void handleSignal(int signum) {
    if (signum == SIGINT || signum == SIGTERM) {
        g_logger->log(LogLevel::WARN, "Received signal. Shutting down.");
        if (g_server)
            g_server->stop();
    }
}

int main(int argc, char* argv[]) {
    try {
        ServerConfig config = ServerConfig::load(argc, argv);

        g_logger = std::make_shared<Logger>();

        if (config.config_file_used.empty()) {
            g_logger->log(LogLevel::INFO, "No config file found, using default settings.");
        } else {
            g_logger->log(LogLevel::INFO, "Config file \"" + config.config_file_used + "\" used.");
        }

        std::shared_ptr<StaticFileHandler> handler =
            std::make_shared<StaticFileHandler>(config.createFileServerConfig());

        g_server = std::make_unique<HttpServer>(handler, g_logger, config.createHttpServerConfig());

        std::signal(SIGINT, handleSignal);
        std::signal(SIGTERM, handleSignal);

        g_server->start();
    } catch (const std::exception& e) {
        g_logger->log(LogLevel::ERR, e.what());
        return 1;
    }
}