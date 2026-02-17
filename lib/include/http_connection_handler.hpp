#pragma once

#include "http_connection.hpp"
#include "ihttp_request_handler.hpp"

#include <functional>
class HttpConnectionHandler {
  public:
    HttpConnectionHandler(std::shared_ptr<IHttpRequestHandler> request_handler, HttpServerConfig& config);

    void handle(std::shared_ptr<HttpConnection> conn) const;

  private:
    std::shared_ptr<IHttpRequestHandler> m_request_handler = nullptr;

    HttpServerConfig m_config;
    HttpProtocol     m_protocol;
};