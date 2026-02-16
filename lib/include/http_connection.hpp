#pragma once

#include "../../server/include/http_request.hpp"
#include "http_protocol.hpp"
#include "http_response.hpp"
#include "socket.hpp"

class HttpConnection {
  public:
    HttpConnection(Socket client, HttpProtocol& protocol, HttpServerConfig& config);

    HttpRequest readRequest();
    void        writeResponse(HttpResponse response);
    void close();

  private:
    Socket m_client;
    HttpProtocol& m_protocol;
    HttpServerConfig& m_config;

    static constexpr size_t CHUNK_SIZE      = 4096;
    static constexpr size_t MAX_HEADER_SIZE = 8192;
    static constexpr size_t MAX_BODY_SIZE   = 10 * 1024 * 1024;
};