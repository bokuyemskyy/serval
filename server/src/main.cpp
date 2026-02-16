
#include "../../lib/include/http_server.hpp"
#include "../../lib/include/http_server_config.hpp"
#include "static_file_server.hpp"

#include <csignal>
#include <memory>

std::unique_ptr<HttpServer> g_server;

void handleSignal(int signum) {
    if (signum == SIGINT || signum == SIGTERM) {
        Logger::log(LogLevel::WARN, "Received signal. Shutting down.");
        if (g_server)
            g_server->stop();
    }
}

int main(int argc, char* argv[]) {
    try {
        HttpServerConfig config = HttpServerConfig::load(argc, argv);

        g_server = std::make_unique<HttpServer>(std::move(config));

        StaticFileHandler handler(".");

        g_server->setRequestHandler(&handler);

        std::signal(SIGINT, handleSignal);
        std::signal(SIGTERM, handleSignal);

        g_server->start();
    } catch (const std::exception& e) {
        Logger::log(LogLevel::ERR, e.what());
        return 1;
    }
}