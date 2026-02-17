#pragma once

#include "http_request.hpp"
#include "http_response.hpp"

class IHttpRequestHandler {
  public:
    virtual HttpResponse handle(const HttpRequest& request) = 0;
    virtual ~IHttpRequestHandler()                          = default;
};