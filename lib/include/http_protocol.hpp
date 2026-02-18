#pragma once

#include "http_request.hpp"
#include "http_response.hpp"
#include "http_server_config.hpp"

#include <string>

class HttpProtocol {
  public:
    HttpProtocol(HttpServerConfig& config);
    HttpRequest parseRequest(const std::string& raw_request) const;
    std::string serializeResponse(const HttpResponse& response) const;
    bool        isRequestComplete(const std::string& raw_request) const;
    bool        shouldKeepAlive(const HttpRequest& request) const;

  private:
    HttpServerConfig& m_config;
};