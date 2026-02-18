#pragma once

#include "http_request.hpp"
#include "http_response.hpp"

/**
 * IHttpRequestHandler is an interface for handling requests.
 * Implement this interface to provide your own logic to process requests.
 */
class IHttpRequestHandler {
  public:
    /**
     * Handles an incoming HTTP request and generates an appropriate HTTP response.
     *
     * This method is called by the HTTP server whenever a request is received. Implementations should examine the
     * request (method, URL, headers, body, etc.) and produce a corresponding HttpResponse.
     *
     * @param request The incoming HttpRequest to process.
     * @return An HttpResponse object representing the server's response to the request.
     *
     * @note Implementations should be thread-safe if you use multiple workers in HttpServer.
     */
    virtual HttpResponse handle(const HttpRequest& request) = 0;

    virtual ~IHttpRequestHandler() = default;
};