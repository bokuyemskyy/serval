#pragma once

#include "http_request.hpp"
#include "http_response.hpp"

class IHttpRequestHandler {
    public:
    virtual HttpResponse handleRequest(const HttpRequest& request) = 0;
    virtual ~IHttpRequestHandler() = default;
};