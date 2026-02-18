#pragma once

#include "http_request.hpp"
#include "http_response.hpp"
#include "http_server_config.hpp"

#include <string>

/**
 * @brief Core HTTP protocol handler.
 *
 * Responsible for parsing raw HTTP requests, serializing responses,
 * and providing utility methods related to the HTTP protocol.
 *
 * @note This class does not manage connections — it purely understands HTTP.
 */
class HttpProtocol {
  public:
    /**
     * @brief Construct a new HttpProtocol instance.
     *
     * @param config Reference to the server configuration.
     */
    HttpProtocol(HttpServerConfig& config);

    /**
     * @brief Parse a raw HTTP request string into an HttpRequest object.
     *
     * @param raw_request The raw request data received from a client.
     * @return HttpRequest Parsed request object.
     *
     * @note Make sure to call isRequestComplete() previously to avoid exceptions.
     */
    HttpRequest parseRequest(const std::string& raw_request) const;

    /**
     * @brief Serialize an HttpResponse into a string to send over the network.
     *
     * @param response The response to serialize.
     * @return std::string Serialized HTTP response string.
     */
    std::string serializeResponse(const HttpResponse& response) const;

    /**
     * @brief Check whether a raw request string contains a complete HTTP request (header and body, if present).
     *
     * @param raw_request The raw request data.
     * @return true If the request is complete.
     * @return false Otherwise.
     */
    bool isRequestComplete(const std::string& raw_request) const;

    /**
     * @brief Determine if the connection should be kept alive for further requests based on the HTTP version.
     *
     * @param request The parsed HTTP request.
     * @return true If the connection should be kept alive.
     * @return false Otherwise.
     */
    bool shouldKeepAlive(const HttpRequest& request) const;

  private:
    HttpServerConfig& m_config;
};