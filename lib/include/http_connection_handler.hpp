#pragma once

#include "../../server/include/http_response.hpp"
#include "http_connection.hpp"
#include "http_request.hpp"
#include "ihttp_request_handler.hpp"

#include <functional>
class HttpConnectionHandler {
  public:
    HttpConnectionHandler();

    void setRequestHandler(IHttpRequestHandler* request_handler);
    void handleConnection(HttpConnection conn);

  private:
  IHttpRequestHandler* m_request_handler = nullptr;
};