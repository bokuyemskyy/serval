
#include "server/config.hpp"
#include "server/http_server.hpp"

#include <memory>

std::unique_ptr<HTTPServer> g_server;

int main(int argc, char* argv[]) {
    Config config = Config::load(argc, argv);

    g_server = std::make_unique<HTTPServer>(config);
}