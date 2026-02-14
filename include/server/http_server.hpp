#pragma once

#include "server/config.hpp"

class HTTPServer {
  public:
    HTTPServer(Config config);
    ~HTTPServer();

  private:
    void run();
    void stop();
};